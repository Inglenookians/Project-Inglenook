/*
* log_client.h: log_client thread safe log writing functionality.
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
#ifndef LOG_CLIENT_H_
#define LOG_CLIENT_H_

// standard library includes
#include <iostream>

// boost (http://boost.org) includes
#include <boost/thread/tss.hpp>

// inglenook includes
#include "log_writer.h"
#include "log_entry_buffered.h"
#include "log_entry_modifiers.h"

namespace inglenook
{

namespace logging
{

/**
 * Log client stream manipulators (log flags [lf])
 * The following flags are used to modify the behavior of the log_client data streams.
 */
enum lf : unsigned int
{
	end = 0x01 /**< End the current log entry and schedule for serialization. */
};

/// shared points to a log buffer
typedef std::shared_ptr<log_entry_buffered> log_buffer;

/// thread specific data is held entirely within this data type.
typedef boost::thread_specific_ptr<log_buffer> ts_log_buffer;

// type defintion needed of a standard stream (cout, cerr, stringstream, etc)
typedef std::basic_ostream<char, std::char_traits<char> > stream_type;

// function signature of std stream manipulators (std::endl, std::end, etc)
typedef stream_type& (*std_stream_manipulator)(stream_type&);

/**
 * The log_client class provides a thread safe log writing interface for client applications.
 * The log_class class acts as a thread safe intermediate between the log_writer and client applications. It can be used
 * either via the add_entry() method call, or the stream operator.
 */
class log_client
{

public:

	// there is no default constructor for this class.
	log_client() = delete;

	// there is no copy constructor for this class.
	log_client(log_client&) = delete;

	/// creates a new log_client, which will emit log entries to
	/// the specified log_writer interface. log_writer cannot be null.
	log_client(std::shared_ptr<log_writer> output_interface);

	/// deconstructs the log_client and releases resources.
	virtual ~log_client();

	// gets the default name space for this thread.
	const std::string& default_namespace() const;

	// unsets the default namespace for this log client (forcing it to
	// fall back on to the log writers default namespace)
	void clear_default_namespace();

	// sets the default name space for this thread.
	void default_namespace(const std::string& value);

	// gets the default entry type for this thread.
	const category& default_entry_type() const;

	// unsets the default entry type for this log client (forcing it to
	// fall back on to the log writers default type)
	void clear_default_entry_type();

 	/// sets the default entry type for this thread.
	void default_entry_type(const category& value);

	/// The internal log entry buffer for this client.
	log_buffer buffer();

    /// Creates a debug log entry
    log_client& debug();

    /// Creates a verbose (trace) log entry
    log_client& trace();

    /// Creates an information log entry
    log_client& info();

    /// Creates a warning log entry
    log_client& warning();

    /// Creates an error log entry
    log_client& error();

    /// Creates a fatal error log entry
    log_client& fatal();

private:


    /// initializes the data buffer for the current context.
    void initialize_buffer();

    /// ensures that the internal buffer is initialized within the current context.
    void check_buffer();

	/// creates a log category of the specified type.
	inline log_client& create_log_stream(category _category);

	/// convenience method to perform null checks on thread specific
	/// buffer prior to stream write. nice and centralized.
    template <class type> inline log_client& send_to_stream(type& x);

    /// pointer to the output interface that this client should submit log
    /// entries to. this is set at construction and shouldn't change.
    std::shared_ptr<log_writer> m_output_interface;

    /// thread specific default entry type.
    boost::thread_specific_ptr<category> m_ts_default_entry_type;

    /// thread specific default name space.
    boost::thread_specific_ptr<std::string> m_ts_default_namespace;

    /// thread specific data buffer used to collate stream input until a
    /// log entry is flushed with lf::end;
	ts_log_buffer m_buffer;

	//
	// THERE IS NOTHING SPECIAL BELOW THIS POINT
	// just a lot of stream operator overloads that redirect (as various types) to the
	// log_client::send_to_stream() method above. nothing to see here. move along.
public:
    log_client& operator<<(std::streambuf* _streambuf_ptr);
    log_client& operator<<(int _int);
    log_client& operator<<(long _long);
    log_client& operator<<(unsigned long _unsigned_long);
    log_client& operator<<(bool _bool);
    log_client& operator<<(short _short);
    log_client& operator<<(unsigned short _unsigned_short);
    log_client& operator<<(unsigned int _unsigned_int);
    log_client& operator<<(char _char);
#ifdef _GLIBCXX_USE_LONG_LONG
    log_client& operator<<(long long _long_long);
    log_client& operator<<(unsigned long long _unsigned_long_long);
#endif
    log_client& operator<<(double _double);
    log_client& operator<<(float _float);
    log_client& operator<<(const void* _const_void_ptr);
    log_client& operator<<(const char* _const_char_ptr);
    log_client& operator<<(std::string _string);
    log_client& operator<<(std::string* _string);
    log_client& operator<<(std::stringstream _stringstream);
    log_client& operator<<(lf _lf);
    log_client& operator<<(const ns& _ns);
    log_client& operator<<(const log_data& _log_data);
    log_client& operator<<(std_stream_manipulator _manipulator);

    /////////////////////////////////////////////////////////////////
    //
};

} // namespace inglenook::logging

} // namespace inglenook

#endif /* LOG_CLIENT_H_ */

