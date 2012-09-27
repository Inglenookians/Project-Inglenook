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

#ifndef LOG_WRITER_H_
#define LOG_WRITER_H_

//
// The following pre-compiler should:
// create a new type definition of 'pid_type' which is appropriate to store the platform specific process identifier.
// create a new definition of 'INGLENOOK_CURRENT_PID' which can be invoked to get the current process id.
// Example:
//	 // get and store current process id.
//   pid_type mypid = INGLENOOK_CURRENT_PID();
//
#ifndef _WIN32

// Linux support (officially tested and maintained).
#  include <sys/types.h>
#  include <unistd.h>
#  include <stdlib.h>
#  define INGLENOOK_CURRENT_PID ( getpid )
   typedef pid_t pid_type;

#else

// Windows support (not tested or maintained).
#  include <windows.h>
#  warning INGLENOOK: WIN32 code has never been tested. This might not even compile. Good luck brave warrior.
#  define INGLENOOK_CURRENT_PID ( GetCurrentProcessId )
   typedef DWORD pid_type;

#endif

// standard library includes
#include <ostream>

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/circular_buffer.hpp>

// inglenook includes
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
 * adheres to the 'Inglenook Logging File Format' (XSD, or xml schema) which is fully defined at http://TODO/meep. This XML is designed to
 * be read by (TODO name of XML reader class here), if also contains an XLT so should be human readable. Clients not capable of linking to
 * libraries (such as scripts) should use (TODO name of log-writer) to handle this.
 */
class log_writer
{

	private:

		// (there is no default constructor for the LogWriter)
		log_writer() = delete;

		// Creates a new LogWriter instance which will emit logs to the specified output stream.
		log_writer(const std::shared_ptr<std::ostream>& output_stream);



	public:

		// (there is no default constructor for the LogWriter)
		log_writer(const log_writer&) = delete;

		// Creates a new log_writer instance.
		static std::shared_ptr<log_writer> create();

		// Creates a new log_writer instance which will emit logs to the specified file.
		static std::shared_ptr<log_writer> create_from_file_path(const boost::filesystem::path& output_file, const bool& create = true);

		// Creates a new log_writer instance which will emit logs to a specified stream
		static std::shared_ptr<log_writer> create_from_stream(const std::shared_ptr<std::ostream>& output_stream);

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


	private:

		/// Gets and returns the current process id.
		static const pid_type get_real_process_id();

		/// Gets and returns the current process name.
		static const std::string get_real_process_name();

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



};

} // namespace inglenook::logging

} // namespace inglenook

#endif /* LOG_WRITER_H_ */
