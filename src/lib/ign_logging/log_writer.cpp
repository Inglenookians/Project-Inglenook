/*
 * log_writer.cpp: log_writer provides log writing functionality for the logging service daemon.
 * Copyright (C) 2012, Project Inglenook (http://www.project-inglenook.co.uk)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// inglenook includes
#include "log_writer.h"
#include "log_exceptions.h"
#include <ign_directories/directories.h>

// standard library includes
#include <fstream>
#include <sstream>

// boost (http://boost.org) includes
#include <boost/locale.hpp>
#include <boost/format.hpp>
#include <boost/exception/all.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/thread/locks.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace inglenook {

namespace logging {

/**
 * Creates a POSIX time item out of a milisecond duration..
 * @param ms milliseconds until event.
 */
const boost::posix_time::ptime timeout_ms(int ms) {
	return boost::get_system_time() + boost::posix_time::milliseconds(ms);
}

/**
 * Creates a new log_writer instance which will emit output to the specified std::ostream.
 * @param output_stream std::ostream to write XML to.
 * @see ~log_writer()
 */
log_writer::log_writer(const std::shared_ptr<std::ostream>& output_stream, bool write_header, bool write_footer) :
	m_log_serialization_thread(nullptr),
	m_log_serialization_shutdown_mutex(new boost::timed_mutex()),
	m_log_serialization_queue_mutex(new boost::mutex()),
	m_log_serialization_element_queuing_mutex(new boost::mutex()),
	m_process_id(inglenook::core::application::pid()),
	m_process_name(inglenook::core::application::name()),
	m_output_stream(output_stream),
	m_xml_serialization_threshold(category::information),
	m_console_serialization_threshold(category::information),
	m_default_entry_type(category::information),
	m_default_namespace("inglenook.anonymous"),
	m_write_header(write_header),
	m_write_footer(write_footer)
	{

	// check the output streams health
	if (m_output_stream == nullptr || m_output_stream->fail()) {
		BOOST_THROW_EXCEPTION(failed_to_create_log_exception()
				<< inglenook_error_number(log_exception_bad_stream));
	}

	// create the transaction buffer.
	m_log_serialization_queue = std::shared_ptr<log_message_queue>(
			new log_message_queue(LOG_WRITER_QUEUE_SIZE));

	// we are good, start the serialization thread.
	m_log_serialization_thread = std::shared_ptr<boost::thread>(
			new boost::thread(&log_writer::_log_serialization_worker, this));

}

/**
 * Creates a new log_writer instance based on inglenooks logging directory structure. This can be impacted by a variety of factors
 * including but not limited to; global configuration, application configuration and environment.
 * @param write_header indicates if the XML preamble should be written on startup
 * @param write_footer indicates if the XML closure tags should be written on shutdown.
 */
std::shared_ptr<log_writer> log_writer::create(bool write_header, bool write_footer)
{

	return log_writer::create_from_file_path(default_log_path(), write_header, write_footer);

}

/**
 * Determines the default log path, note that the file path is time dependant and will
 * change with every call made to it.
 * @returns the suggested log path.
 */
boost::filesystem::path log_writer::default_log_path()
{

	using namespace inglenook::core;

	std::stringstream filename_buffer;

	// todo ild-locate here, for now hard code the inglenook directory
	filename_buffer << inglenook::directories::log().native() << "/";

	// append the process name
	filename_buffer << application::name() << "/";

	// append a reverse time stamp to the pid so file name format becomes
	// pid_yyyymmdd_hhmmss.xml, as per the specifications. time is UTC.
	filename_buffer.imbue(std::locale(filename_buffer.getloc(),
			new boost::posix_time::time_facet("%Y%m%d-%H%M%S")));
	filename_buffer << boost::posix_time::second_clock::universal_time();

	// get the current process id.
	filename_buffer << "-" << application::pid();

	// add file extension
	filename_buffer << ".xml";

	return filename_buffer.str();
}

/**
 * Creates a new log_writer instance which will emit output to the specified file.
 * @param output_file Path identifying file to write XML to.
 * @param create If output_file doesn't exist, indicates whether to create it on the fly. Defaults to true.
 * @param write_header indicates if the XML preamble should be written on startup
 * @param write_footer indicates if the XML closure tags should be written on shutdown.
 */
std::shared_ptr<log_writer> log_writer::create_from_file_path(
		const boost::filesystem::path& output_file, const bool& create,
		bool write_header, bool write_footer)
{
	try
	{
		using namespace boost::filesystem;

		// determine if the file exists
		if (!exists(output_file)) {
			// the file doesn't exist.
			// check to see if we should create it...
			if (create) {

				//
				// ideally - I would like to check the validity of the requested file here but
				// boost::filesystem::native() is having some problems with relative paths such
				// as "./log.xml" (incl. the leading ./); which is apparently not a valid file path.
				// instead we'll have to push on and rely on the resulting exception to indicate bad files.
				//

				// determine the parent directory path (required for relative paths)
				auto parent_directory = absolute(output_file).branch_path();
				boost::system::error_code filesystem_error;

				// ensure parent directory exists, or create it
				if (!exists(parent_directory) && !create_directories(
						parent_directory, filesystem_error)) {
					// the parent path doesn't exist and we failed to create it.
					BOOST_THROW_EXCEPTION(failed_to_create_log_exception()
							<< boost_filesystem_error(filesystem_error)
							<< inglenook_error_number(log_exception_bad_file_path));
				}

			}
else				// the file does not exist, and we were not instructed to create it.
				BOOST_THROW_EXCEPTION(log_not_found_exception()
						<< inglenook_error_number(log_exception_bad_file_path));

			}

			// at this point either the log exists, or we are good to create it
			// so attempt to open the specified file path for appending data.
			return std::shared_ptr<log_writer>(new log_writer(std::shared_ptr<std::ofstream>(
									new std::ofstream(output_file.native(), std::ios::app)),
									write_header, write_footer));

		}
		catch(boost::exception& ex)
		{
			// if anything goes wrong augment exception with the file path we were working with.
			ex << log_file_name(output_file);
			throw;
		}
	}

	/**
	 * Creates a new log_writer instance which will emit output to the specified output stream.
	 * @param output_stream stream to write XML to.
	 * @param write_header indicates if the XML preamble should be written on startup
	 * @param write_footer indicates if the XML closure tags should be written on shutdown.
	 */
	std::shared_ptr<log_writer> log_writer::create_from_stream(const std::shared_ptr<std::ostream>& output_stream,
			bool write_header, bool write_footer)
	{
		// at this point either the log exists, or we are good to create it
		// so attempt to open the specified file path for appending data.
		return std::shared_ptr<log_writer>(new log_writer(output_stream, write_header, write_footer));
	}

	/**
	 * Deconstructs the log_writer, releasing any associated resources.
	 * @see log_writer()
	 */
	log_writer::~log_writer()
	{

		// nothing to do you - m_output_stream should close itself if sharedptr's have expired.
		if(m_log_serialization_thread != nullptr)
		{
			// attempt to acquire the lock on the shutdown mutex
			boost::timed_mutex::scoped_lock lock_shutdown(
					(*m_log_serialization_shutdown_mutex.get()),
					timeout_ms(LOCK_SHUTDOWN_TIMEOUT));

			// check we asserted lock ownership
			if(!lock_shutdown.owns_lock())
			{
				// throw an exception - indicating that the fault was a lock acquisition issue.
				BOOST_THROW_EXCEPTION( log_serialization_exception()
						<< inglenook_error_number(unable_to_aquire_shutdown_lock) );
			}

			// notify serialization worker its time for shutdown...
			m_log_serialization_worker_shutdown = true;

			// release the lock
			lock_shutdown.unlock();

			// wait for serialization thread to close.
			m_log_serialization_thread->join();

		}

	}

	/**
	 * Adds a log entry to the serialization queue.
	 * This method is responsible for submitting the item to the serialization queue.
	 * It is important to note that the serialization worker accesses entry in a non-thread safe manner. the assumption is that
	 * after the entry has been passed to this method and enqueued successfully, entry is no longer your responsibility and should
	 * not be handled by any other threads but serialization (which will release the resource when it is finished). TLDR; DO NOT
	 * USE [entry] AFTER A SUCCESSFUL CALL TO THIS METHOD.
	 * @param entry log entry to enqueue and serialize
	 * @returns true if the item is enqueued.
	 */
	bool log_writer::add_entry(std::shared_ptr<log_entry>& entry)
	{

		bool entry_scheduled = false;

		// make sure there is a message
		if(entry->message().length() > 0)
		{
			// correct empty name spaces.
			if(entry->log_namespace().length() == 0)
				entry->log_namespace(default_namespace());

			int schedule_attempts = 0;
			const int MAX_SCHEDULE_ATTEMPTS = 10;

			// attempt to acquire the lock on the queue mutex
			boost::mutex::scoped_lock lock_queue(
					(*m_log_serialization_queue_mutex.get()));

			// check we asserted lock ownership
			if(lock_queue.owns_lock())
			{

				// attempt to schedule this entry...
				while(schedule_attempts++ < MAX_SCHEDULE_ATTEMPTS)
				{

					bool boolean_queue = true;

					// if the queue is empty
					if(m_log_serialization_queue->empty())
					{
						// acquire the queue notification mutex
						boost::mutex::scoped_lock lock_notify((*m_log_serialization_element_queuing_mutex.get()));
						m_log_serialization_element_queuing.notify_all();
					}
					// else we'll attempt to schedule based on the queue size
					else boolean_queue = !m_log_serialization_queue->full();

					// make sure the queue isn't full
					if(boolean_queue)
					{
						// push the item on to the queue
						m_log_serialization_queue->push_back(entry);
						entry_scheduled = true;
						break;
					}

					//
					// the queue is full. yield time slice to let other threads do some work
					// (most importantly the log writer thread!).
					m_log_serialization_element_serialized.timed_wait(lock_queue, timeout_ms(RESCHEDULE_MAX_RETRY_DELAY));
				}

			}

			// check if we failed to schedule to entry.
			if(!entry_scheduled)
			{
				// for now out good friend std::cerr, may want to make this throw in the future.
				// currently not hitting this event, even under the most obscene load.
				std::cerr << "WARNING: failed to schedule log entry." << std::endl;
			}

		}

		// return result
		return entry_scheduled;

	}

	/**
	 * Gets the process id
	 * This is the id of the parent process and is self determined by the class. It shouldn't be modified.
	 * @returns The process id of this process.
	 */
	const pid_type log_writer::pid() const
	{
		// return the process id
		return m_process_id;
	}

	/**
	 * Gets the process name
	 * This is the name of the process we are logging on behalf of. This can be modified, but only during instantiation
	 * Note that this might not be the same as the current process name, and as such might not correlate to the process
	 * identified by log_writer::get_real_process_name().
	 * @returns The name of the process this transaction is acting on behalf of.
	 */
	const std::string log_writer::process_name() const
	{
		// return the name of the process.
		return m_process_name;
	}


	/**
	 * This method writes out the XML header.
	 * This includes the XML declaration, root node (tied in the XSD) and the binary information element.
	 */
	void log_writer::write_xml_header()
	{

		// locate the logging xsd
		std::stringstream xsd_location;
		xsd_location << "http://schemas.project-inglenook.co.uk/" // domain for schema's
		<< "0.00-DEVELOPMENT/" // software version
		<< "schemas/" // keep top level tidy for html.
		<< "file-formats/" // types of schema
		<< "inglenook-log-file.xsd"; // specific file.

		// just because the binary name can be tampered with..
		std::string safe_process_name = process_name();
		boost::replace_all(safe_process_name, "<", "&lt;");
		boost::replace_all(safe_process_name, ">", "&gt;");

		// write out the xml data type declaration
		(*m_output_stream.get()) << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

		// write out the root node and type in the xsd
		(*m_output_stream.get()) << "<inglenook-log-file xmlns=\"" << xsd_location.str() << "\">";

		// write out binary information block.
		(*m_output_stream.get()) << "<process-id pid=\"" << pid() << "\">";
		(*m_output_stream.get()) << "<binary-name><![CDATA[" << safe_process_name << "]]></binary-name>";
		(*m_output_stream.get()) << "<binary-version><![CDATA[" << "#.## DEVELOPMENT" << "]]></binary-version>";
		(*m_output_stream.get()) << "<log-writer-version><![CDATA[" << "#.## DEVELOPMENT" << "]]></log-writer-version>";
		(*m_output_stream.get()) << "</process-id>";
		(*m_output_stream.get()) << "<log-entries>";
	}

	/**
	 * Get the current process name [NOTE: Platform Specified Code].
	 * This method resorts to platform specific code to attempt to self determine the process name.
	 * @returns The name of the current process, or empty string on failure.
	 */
	void log_writer::_log_serialization_worker()
	{

		try
		{

			// attempt to acquire the lock on the shutdown mutex
			boost::mutex::scoped_lock lock_item_waiting(
					(*m_log_serialization_element_queuing_mutex.get()));

			// make sure this thread owns the notification lock.
			if(!lock_item_waiting.owns_lock())
			BOOST_THROW_EXCEPTION( log_serialization_exception()
					<< inglenook_error_number(unable_to_aquire_queue_notification_lock) );

			// start the log file.
			if(m_write_header) write_xml_header();

			while(true)
			{

				//
				// while there are log entries in the serialization queue, pop them
				// off and process them, as configured to do so.
				//

				std::shared_ptr<log_entry> entry;
				while((entry = _log_serialization_worker_next_entry()) != nullptr)
				{

					// make sure the entry is filled out.
					if(entry->entry_type() != category::unspecified &&
					   entry->entry_type() != category::no_log &&
					   entry->log_namespace().length() > 0 &&
					   entry->message().length() > 0)
					{
						if(entry->entry_type() >= xml_threshold())
							_log_serialization_worker_serialize(entry);

						if(entry->entry_type() >= console_threshold())
							_log_serialization_worker_screen(entry);
					}
				}

				//
				// the queue is empty, use this breathing time to check to see if the
				// shutdown flag is set, if so we'll want to initialize a thread shutdown,
				// else we will enter a sleep state for a new ms to lighten cpu load
				// before we re-check the queue for contents.
				//

				// attempt to acquire the lock on the shutdown mutex
				boost::timed_mutex::scoped_lock lock_shutdown(
						(*m_log_serialization_shutdown_mutex.get()),
						timeout_ms(LOCK_SHUTDOWN_TIMEOUT));

				// check we asserted lock ownership
				if(!lock_shutdown.owns_lock())
				{
					// throw an exception - indicating that the fault was a lock acquisition issue.
					BOOST_THROW_EXCEPTION( log_serialization_exception()
							<< inglenook_error_number(unable_to_aquire_shutdown_lock) );
				}

				// if we have been told to shutdown - exit loop
				// deconstruction of lock_shutdown will release mutex.
				if(m_log_serialization_worker_shutdown) break;

				// release the lock
				lock_shutdown.unlock();

				// we are idle, nothing to do so sleep for a bit. siesta!
				// (I envisage, just waiting around for a few ms to be more efficient than blocking the thread
				// on a conditional variable and notifying it when the queue has some new contents to process).
				m_log_serialization_element_queuing.timed_wait(lock_item_waiting, timeout_ms(SERIALIZER_IDLE_TIMEOUT));

			}
		}
		catch(boost::exception&)
		{
			// if the serializer crashes we can't write a log entry, best we can muster is standard error.
			std::cerr << boost::locale::translate("ERROR: Log serialization has failed.") << std::endl;
			std::cerr << boost::current_exception_diagnostic_information() << std::endl;
		}

		//
		// shutdown serialization.
		//
		try
		{
			// if the header is set to be written.
			if(m_write_footer)
			{
				(*m_output_stream.get()) << "</log-entries>";
				(*m_output_stream.get()) << "</inglenook-log-file>";
			}
		}
		catch(...) { /* if we crashed because of a bad stream, don't make the problem worse */}

	}

	/**
	 * Serializes an entry to the output stream as XML.
	 * Given a pointer to a log entry, serializes the item to the output stream as XML. This should only
	 * ever be called by the serialization worker thread.
	 * @param entry entry to serialize.
	 */
	void log_writer::_log_serialization_worker_serialize(std::shared_ptr<log_entry> entry)
	{


		// should only need to initialize this stuff once for performance
		auto output_stream = m_output_stream.get();
		auto timestamp_formatter = std::locale(output_stream->getloc(),
				new boost::posix_time::time_facet("%Y-%m-%dT%H:%M:%sZ"));

		/*
		 * This is what we are aiming for:

		 <log-entry timestamp="2012-12-21T00:00:00.00Z" severity="4" ns="inglenook.sample.process">
		 <message><![CDATA[Yikes! Something incredibly Mayan happened to the process - cannot continue.]]></message>
		 <extended-data>
		 <item key="sample.specific"><![CDATA[Kittens]]></item>
		 <item key="sample.host"><![CDATA[127.0.0.1]]></item>
		 <extended-data>
		 </log-entry>

		 */

		// sanitize message this can contain user input
		std::string message = entry->message();
		boost::replace_all(message, "<", "&lt;");
		boost::replace_all(message, ">", "&gt;");

		// get the extended data (pre-doing this to keep xml writing as clean as possible).
		auto extended_data = entry->extended_data();

		// open the <log-entry> dom element
		*output_stream << "<log-entry timestamp=\"";
		output_stream->imbue(timestamp_formatter);
		*output_stream << boost::posix_time::second_clock::universal_time();
		*output_stream << "\" severity=\"" << entry->entry_type() << "\" ns=\"" << entry->log_namespace() << "\">";

		// output the message body
		*output_stream << "<message><![CDATA[" << message << "]]></message>";

		// check for extended data
		if(extended_data.size() > 0)
		{

			// start the <extended-data> dom item
			*output_stream << "<extended-data>";

			// iterate through all the data in the map
			for(auto data = extended_data.begin(); data != extended_data.end(); data++)
			{

				// sanitize the data provided (not only value is sanitized).
				std::string value = data->second;
				boost::replace_all(value, "<", "&lt;");
				boost::replace_all(value, ">", "&gt;");

				// and write it out.
				*output_stream << "<item key=\"" << data->first << "\"><![CDATA[" << value << "]]></item>";

			}

			// end the <extended-data> dom item
			*output_stream << "</extended-data>";

		}

		// close the <log-entry>
		*output_stream << "</log-entry>";

	}

	/**
	 * Serializes an entry to the console.
	 * Given a pointer to a log entry, serializes the item to the console. This should only
	 * ever be called by the serialization worker thread.
	 * @param entry entry to serialize.
	 */
	void log_writer::_log_serialization_worker_screen(std::shared_ptr<log_entry> entry)
	{

		/*
		 // both cout and ceer should have the same locale...
		 auto timestamp_formatter = std::locale(std::cout.getloc(),
		 new boost::posix_time::time_facet("%d-%M-%Y-%H:%M:%S"));

		 // short names for all the entry types localized for user.
		 std::string categories[] =
		 {
		 boost::locale::translate("     "),
		 boost::locale::translate("debug"),
		 boost::locale::translate("trace"),
		 boost::locale::translate(" info"),
		 boost::locale::translate(" warn"),
		 boost::locale::translate("error"),
		 boost::locale::translate("fatal")
		 };

		 // choose a category string for the entry
		 std::string category = entry->entry_type() < sizeof(categories) ?
		 categories[entry->entry_type()] : categories[0];
		 */

		// determine which stream we need to push this message to.
		std::ostream *output_stream = entry->entry_type() >= LOG_CATEGORY_CERR_BOUNTRY ?
		&std::cerr : &std::cout;
		/*
		 // push it all out to console.
		 *output_stream << "[";
		 output_stream->imbue(timestamp_formatter);
		 *output_stream << boost::posix_time::second_clock::local_time();
		 *output_stream << " " << category << "] ";
		 */
		*output_stream << entry->message() << std::endl;

	}

	/**
	 * Gets the next item off the queue for serialization.
	 * This method will in a thread safe manner, get the next item off the queue for processing. If there is no item
	 * available for processing the method will return nullptr. If a lock on the queue's mutex cannot be obtained
	 * it will also return nullptr. If the thread successively fails to acquire a lock on the queue mutex, the queue
	 * will eventually fill resulting in an exception in client processes. If this becomes a problem we may want to
	 * allow the writer to wait on the lock longer than clients.
	 * @returns next log entry for serialization, or nullptr if unavailable.
	 */
	std::shared_ptr<log_entry> log_writer::_log_serialization_worker_next_entry()
	{

		std::shared_ptr<log_entry> result = nullptr;

		// attempt to acquire the lock on the queue mutex
		boost::mutex::scoped_lock lock_queue(
				(*m_log_serialization_queue_mutex.get()));

		// check we asserted lock ownership
		if(lock_queue.owns_lock())
		{
			// if the queue isn't empty
			if(!m_log_serialization_queue->empty())
			{
				// pop an item off the front of the queue...
				result = m_log_serialization_queue->front();
				m_log_serialization_queue->pop_front();

				// notify someone queing (if anyone is) that space has become available.
				m_log_serialization_element_serialized.notify_one();

			}

		}

		// return the result.
		return result;

	}

	/**
	 * Gets the value for the default name space
	 * This property makes no guarantees of thread safety.
	 * @returns value of the property
	 */
	const std::string& log_writer::default_namespace() const
	{
		return m_default_namespace;
	}

	/**
	 * Sets the value for the default name space
	 * This property makes no guarantees of thread safety.
	 * @param value new value for the property
	 */
	void log_writer::default_namespace(const std::string& value)
	{
		m_default_namespace = value;
	}

	/**
	 * Gets the value for the default entry type
	 * This property makes no guarantees of thread safety.
	 * @returns value of the property
	 */
	const category& log_writer::default_entry_type() const
	{
		return m_default_entry_type;
	}

	/**
	 * Sets the value for the default entry type
	 * This property makes no guarantees of thread safety.
	 * @param value new value for the property
	 */
	void log_writer::default_entry_type(const category& value)
	{
		m_default_entry_type = value;
	}

	/**
	 * Gets the value of the xml serialization threshold.
	 * This property makes no guarantees of thread safety. The writer consults this property
	 * when deciding whether an entry can be serialized to XML. Properties lower than this
	 * threshold (more verbose) will not be emitted.
	 * @returns the current value of the property
	 */
	const category& log_writer::xml_threshold() const
	{
		return m_xml_serialization_threshold;
	}

	/**
	 * Sets the value for the xml serialization threshold.
	 * This property makes no guarantees of thread safety. The writer consults this property
	 * when deciding whether an entry can be serialized to XML. Properties lower than this
	 * threshold (more verbose) will not be emitted.
	 * @param value new value for the property
	 */
	void log_writer::xml_threshold(const category& value)
	{
		m_xml_serialization_threshold = value;
	}

	/**
	 * Gets the value of the console serialization threshold.
	 * This property makes no guarantees of thread safety. The writer consults this property
	 * when deciding whether an entry can be serialized to the console. Properties lower than this
	 * threshold (more verbose) will not be emitted.
	 * @returns the current value of the property
	 */
	const category& log_writer::console_threshold() const
	{
		return m_console_serialization_threshold;
	}

	/**
	 * Sets the value for the console serialization threshold.
	 * This property makes no guarantees of thread safety. The writer consults this property
	 * when deciding whether an entry can be serialized to the console. Properties lower than this
	 * threshold (more verbose) will not be emitted.
	 * @param value new value for the property
	 */
	void log_writer::console_threshold(const category& value)
	{
		m_console_serialization_threshold = value;
	}


} // namespace inglenook::logging

} // namespace inglenook
