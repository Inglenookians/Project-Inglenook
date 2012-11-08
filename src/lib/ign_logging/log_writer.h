/*
* log_writer.h: log_writer provides log writing functionality for the logging service daemon.
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

#pragma once

// standard library includes
#include <ostream>

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/circular_buffer.hpp>

// inglenook includes
#include <ign_core/application.h>
#include "log_entry.h"

namespace inglenook
{

namespace logging
{

/// log message queue used to schedule message serialization.
typedef boost::circular_buffer<std::shared_ptr<log_entry>> log_message_queue;

/**
 * The log_writer class provides log writing functionality for client applications.
 * The log_writer class will write log entries as well formed XML to a specified output stream (std::ostream). XML emitted by this class
 * adheres to the 'Inglenook Logging File Format' (XSD, or xml schema) which is fully defined by the XSD in the XML header. This XML is designed to
 * be read by inglenook system tools, if also may contain an XLT so should be human readable.
 */
class log_writer
{

	protected:

		// (there is no default constructor for the LogWriter)
		log_writer() = delete;

		// Creates a new LogWriter instance which will emit logs to the specified output stream.
		log_writer(const std::shared_ptr<std::ostream>& output_stream, const bool& write_header = true, const bool& write_footer = true,
				const pid_type& pid = NO_PID, const std::string& application_name = "");



	public:

		// (there is no default constructor for the LogWriter)
		log_writer(const log_writer&) = delete;

		// Creates a new log_writer instance.
		static std::shared_ptr<log_writer> create(const bool& write_header = true, const bool& write_footer = true,
				const pid_type& pid = NO_PID, const std::string& application_name = "");

		// Creates a new log_writer instance which will emit logs to the specified file.
		static std::shared_ptr<log_writer> create_from_file_path(const boost::filesystem::path& output_file, const bool& create = true,
				const bool& write_header = true, const bool& write_footer = true,
				const pid_type& pid = NO_PID, const std::string& application_name = "");

		// Creates a new log_writer instance which will emit logs to a specified stream
		static std::shared_ptr<log_writer> create_from_stream(const std::shared_ptr<std::ostream>& output_stream,
				const bool& write_header = true, const bool& write_footer = true,
				const pid_type& pid = NO_PID, const std::string& application_name = "");

		// gets the default log path for this run of the software.
		static boost::filesystem::path default_log_path(const pid_type& pid = NO_PID, const std::string& application_name = "");

		// Destroys the LogWriter, releasing any associated resources.
		virtual ~log_writer();



		/// schedules an entry for addition to the log
		/// IMPORTANT: please see method implementation header for important information about this method.
		///   you probably shouldn't be calling it directly anyhow.
		bool add_entry(std::shared_ptr<log_entry>& entry);

		/// gets the current process id.
		const pid_type pid() const;

		/// gets the current process name (or name of process we are logging on behalf of)
		const std::string process_name() const;

		// gets the default name space
		const std::string& default_namespace() const;

		// sets the default name space
		void default_namespace(const std::string& value);

		// gets the default entry type
		const category& default_entry_type() const;

		/// sets the default entry type
		void default_entry_type(const category& value);

		/// gets the xml threshold
		const category& xml_threshold() const;

		/// sets the xml threshold
		void xml_threshold(const category& value);

		/// gets the console threshold
		const category& console_threshold() const;

		/// sets the console threshold
		void console_threshold(const category& value);



		/// defines the value that expresses no PID
		static const pid_type NO_PID;



	private:

		/// Writes the xml header out to the stream.
		void write_xml_header();

		/// amount of time a worker should wait for a space availability notification from the serializer
		/// before it just tries to reschedule the message anyway.
		const int RESCHEDULE_MAX_RETRY_DELAY = 250; //ms (0.25 seconds);

		/// amount of time that the log writer will wait idly, before checking if there are
		/// any messages to serialize. It can be prematurely awoke using m_log_serialization_element_queued
		/// if required to serialize a message (this timeout is mearly a fallback / fail safe).
		const int SERIALIZER_IDLE_TIMEOUT = 250; // ms (0.25seconds)

		/// The amount of time (in milliseconds) that a lock should wait to acquire
		/// m_log_serialization_shutdown_mutex, before giving up and raising an error.
		const int LOCK_SHUTDOWN_TIMEOUT = 1000; // milliseconds (1secs)

		/// the amount of time that a the any thread will wait to acquire the "item queued"
		/// mutex. the serialization thread will abort if it cannot acquire a lock, worker
		/// threads will ignore the condition and accept that they will need to wait.
		const int LOCK_ITEM_QUEUED_TIMEOUT = 250; // ms (0.25seconds)

		/// The number of elements to store in the log writer queue. Changing this
		/// number will impact the memory footprint of all applications. We may want
		/// to make this an option in future versions of the writer.
		const int LOG_WRITER_QUEUE_SIZE = 50; // log_entries

		/// identifies the boundary at an element is no longer printed to cout on a
		/// call to _log_serialization_worker_screen, but cerr instead.
		const category LOG_CATEGORY_CERR_BOUNTRY = category::warning;



		/// (worker thread) serializes logs and writes to output stream. Not to be run
		/// except by the m_log_serialization_thread thread object.
		void _log_serialization_worker();

		/// pops an item off the queue for serialization.
		std::shared_ptr<log_entry> _log_serialization_worker_next_entry();

		/// serializes a log entry and writes it to stream.
		void _log_serialization_worker_serialize(std::shared_ptr<log_entry> entry);

		/// serializes a log entry to standard outputs (cout/cerr)
		void _log_serialization_worker_screen(std::shared_ptr<log_entry> entry);

		/// log serialization thread (see declaration for details).
		std::shared_ptr<boost::thread> m_log_serialization_thread;

		/// indicate is the serialization work thread should shut down.
		/// always acquire ownership of m_log_serialization_shutdown_mutex before use.
		bool m_log_serialization_worker_shutdown = false;

		/// mutex that must be acquired to work with the shutdown variable.
		std::shared_ptr<boost::timed_mutex> m_log_serialization_shutdown_mutex;

		/// queue of messages that are ready for serialization / processing.
		/// always acquire ownership of m_log_serialization_queue_mutex before use.
		std::shared_ptr<log_message_queue> m_log_serialization_queue;

		/// mutex that must be acquired to work with the shutdown variable.
		std::shared_ptr<boost::mutex> m_log_serialization_queue_mutex;

		/// mutex that must be acquired to work with the element queued notification system.
		std::shared_ptr<boost::mutex> m_log_serialization_element_queuing_mutex;

		/// used to notify the serialization thread that it needs to process some information.
		boost::condition_variable m_log_serialization_element_queuing;

		/// used to notify the queuing entries that space in the queue has become available.
		boost::condition_variable m_log_serialization_element_serialized;



		/// current process id
		pid_type m_process_id;

		/// process name (as defined during construction)
		std::string m_process_name;

		/// output stream to write log messages to
		std::shared_ptr<std::ostream> m_output_stream;

		/// lowest type of information that will be written to xml
		category m_xml_serialization_threshold;

		/// lowest type of information that will be written to console
		category m_console_serialization_threshold;

	    /// global entry type.
	    category m_default_entry_type;

	    /// global default name space.
	    std::string m_default_namespace;

	    /// inidicates if a header should be written on startup.
	    bool m_write_header;

	    /// inidicates if a footer should be written on shutdown.
	    bool m_write_footer;


};

} // namespace inglenook::logging

} // namespace inglenook
