/*
* log_client.cpp: log_client thread safe log writing functionality.
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

// boost (http://boost.org) includes
#include <boost/locale.hpp>

// inglenook includes
#include "log_client.h"


namespace inglenook
{

namespace logging
{

/**
 * Creates a new log_client object.
 * Initializes a new log client to be use in conjunction with the specified log_writer output interface.
 * @params output_interface log_writer that completed entries should be written to.
 */
log_client::log_client(std::shared_ptr<log_writer> output_interface)
    : m_output_interface(output_interface)
{
    // nothing to do at the moment.
}

/**
 * Deconstructs the log_client object releasing any resources.
 */
log_client::~log_client()
{
    // nothing to do at the moment.
}

/**
 * Initializes the internal data buffer. .
 */
void log_client::initialize_buffer()
{
    // create a new log entry (this is now a tls_ptr to a shared_ptr to a buffer).
    m_buffer.reset(new std::shared_ptr<log_entry_buffered>(new log_entry_buffered()));
}

/**
 * Initializes the internal data buffer if it is not already initialized for this thread.
 */
void log_client::check_buffer()
{
    // if the buffer has not been initialized for this thread...
    if(m_buffer.get() == nullptr)
    {
        // ...  then initialize it.
        initialize_buffer();
    }
}

/**
 * Gets the value for the default log namespace for this thread.
 * @returns value of the property
 */
const std::string& log_client::default_namespace() const
{
    //
    // don't like multiple returns, but dont want to create
    // local - as this can't be safely used as out by ref.

    // check if a class level override exists...
    if (m_ts_default_namespace.get() != nullptr)
    {
        // .. and if so use it.
        return *m_ts_default_namespace;
    }
    else
    {
        // .. else return the default value
        return m_output_interface->default_namespace();
    }
}

/**
 * Clears the default namespace, forcing the log client to fall back on to its associated
 * log writers default namespace
 */
void log_client::clear_default_namespace()
{
    // update default namespace
    m_ts_default_namespace.reset(nullptr);
}

/**
 * Sets the value of the default name space for this thread.
 * @params value new value for this property
 */
void log_client::default_namespace(const std::string& value)
{
    // update default namespace
    if(m_ts_default_namespace.get() == nullptr)
    {
        m_ts_default_namespace.reset(new std::string());
    }
    *m_ts_default_namespace = value;
}

/**
 * Gets the value for the default entry type for this thread.
 * @returns value of the property
 */
const category& log_client::default_entry_type() const
{
    // check if a default is set, else fall back to writers.
    if (m_ts_default_entry_type.get() != nullptr)
    {
        return *m_ts_default_entry_type;
    }
    else
    {
        return m_output_interface->default_entry_type();
    }
}

/**
 * Clears the default entry type, forcing the log client to fall back on to its associated
 * log writers default value
 */
void log_client::clear_default_entry_type()
{
    // update default entry types
    m_ts_default_entry_type.reset(nullptr);
}


/**
 * Sets the value of the default entry type for this thread.
 * @params value new value for this property
 */
void log_client::default_entry_type(const category& value)
{
    // update default entry types
    if(m_ts_default_entry_type.get() == nullptr)
    {
        m_ts_default_entry_type.reset(new category);
    }

    *m_ts_default_entry_type = value;
}

/**
 * Sends a data element to the message stream.
 * @param type type of object that is being sent to stream.
 * @param x actual variable to send to stream.
 * @returns always returns *this.
 */
template <class type> log_client& log_client::send_to_stream(type& x)
{
    // make sure buffers initialized
    check_buffer();

    // push element in the message stream
    (m_buffer->get()->message_buffer()) << x;

    return *this;
}

/**
 * Sets the current message category to the specified category.
 * @param _category category (or entry type) to set this message as.
 * @returns  always returns *this.
 */
log_client& log_client::create_log_stream(category _category)
{
    // make sure buffers initialized
    check_buffer();

    // set the initial category
    m_buffer->get()->entry_type(_category);

    // return the message stream (even though its not entirely what we are currently
    // working with it is the most relevant, usable std::ostream.
    return *this;
}

/**
 * Gets the internal log entry buffer.
 * @returns  internal log entry buffer.
 */
log_buffer& log_client::buffer()
{
    check_buffer();
    return *m_buffer.get();
}


/// Create a debug log entry
log_client& log_client::debug() { return create_log_stream(category::debugging); }

/// Creates a verbose (trace) log entry
log_client& log_client::trace() { return create_log_stream(category::verbose); }

/// Creates an information log entry
log_client& log_client::info() { return create_log_stream(category::information); }

/// Creates a warning log entry
log_client& log_client::warning() { return create_log_stream(category::warning); }

/// Creates an error log entry
log_client& log_client::error() { return create_log_stream(category::error); }

/// Creates a fatal error log entry
log_client& log_client::fatal() { return create_log_stream(category::fatal); }

/**
 * processes a log data stream manipulator.
 * @param _log_data log data to embue this message with.
 * @returns always returns *this
 **/
log_client& log_client::operator<<(const log_data& _log_data)
{
    // make sure buffers initialized
    check_buffer();

    // update namespace
    m_buffer->get()->extended_data(_log_data.key(), _log_data.value());

    // return the stream
    return *this;
}

/**
 * processes a name space (ns) stream manipulator.
 * @param ns new name space to apply
 * @returns always returns *this
 **/
log_client& log_client::operator<<(const ns& _ns)
{
    // make sure buffers initialized
    check_buffer();

    // update namespace
    m_buffer->get()->log_namespace(_ns.log_namespace());

    // return the stream
    return *this;
}

/**
 * processes a log file (lf) stream manipulator.
 * @param _lf Path identifying file to write XML to.
 * @returns always returns *this
 **/
log_client& log_client::operator<<(lf _lf)
{
    // make sure buffers initialized
    check_buffer();

    // decide how to handle modifier.
    switch(_lf)
    {
        // end entry and flush (lf::end)
        case (lf::end):
        {
            // add the entry to the log schedule and create next entry.
            auto converted_buffer = std::dynamic_pointer_cast<log_entry>(*m_buffer);

            // ensure that the entry type is set...
            if(converted_buffer->entry_type() == category::unspecified)
            {
                // ... else use the fallback type
                converted_buffer->entry_type(default_entry_type());
            }

            // ensure that the entry namespace is set...
            if(converted_buffer->log_namespace() == "")
            {
                // ... else ue the fallback namespace
                converted_buffer->log_namespace(default_namespace());
            }

            // schedule the entry for serialization and re-initialize buffer
            m_output_interface->add_entry(converted_buffer);
            initialize_buffer();

            break;
        }

        // unknown operator
        default:
        {
#ifdef _DEBUG
            // if we are debugging - notify developer that the flow is unrecognized and going unprocessed.
            std::cerr << translate("WARNING: unknown inglenook log flag encountered in stream (inglenook::logging::lf): ") << _lf << std::endl;
#endif
        }
    }

    return *this;
}
//
// THERE IS NOTHING SPECIAL BELOW THIS POINT
// just a lot of stream operator overloads that redirect (as various types) to the
// log_client::send_to_stream() method above. nothing to see here. move along.
////////////////////////////////////////////////////////////////////////////////////////
log_client& log_client::operator<<(int _int)
{ return send_to_stream(_int) ;}

log_client& log_client::operator<<(long _long)
{ return send_to_stream(_long) ;}

log_client& log_client::operator<<(unsigned long _unsigned_long)
{ return send_to_stream(_unsigned_long) ;}

log_client& log_client::operator<<(short _short)
{ return send_to_stream(_short) ;}

log_client& log_client::operator<<(unsigned short _unsigned_short)
{ return send_to_stream(_unsigned_short) ;}

log_client& log_client::operator<<(unsigned int _unsigned_int)
{ return send_to_stream(_unsigned_int) ;}

#ifdef _GLIBCXX_USE_LONG_LONG

log_client& log_client::operator<<(long long _long_long)
{ return send_to_stream(_long_long) ;}

log_client& log_client::operator<<(unsigned long long _unsigned_long_long)
{ return send_to_stream(_unsigned_long_long) ;}

#endif

log_client& log_client::operator<<(double _double)
{ return send_to_stream(_double) ;}

log_client& log_client::operator<<(long double _long_double)
{ return send_to_stream(_long_double) ; }

log_client& log_client::operator<<(float _float)
{ return send_to_stream(_float) ;}

log_client& log_client::operator<<(char _char)
{ return send_to_stream(_char) ;}

log_client& log_client::operator<<(unsigned char _unsigned_char)
{ return send_to_stream(_unsigned_char) ;}

log_client& log_client::operator<<(signed char _signed_char)
{ return send_to_stream(_signed_char) ;}

log_client& log_client::operator<<(const signed char* _const_signed_char_ptr)
{ return send_to_stream(_const_signed_char_ptr) ;}

log_client& log_client::operator<<(const unsigned char* _const_unsigned_char_ptr)
{ return send_to_stream(_const_unsigned_char_ptr) ;}

log_client& log_client::operator<<(std::string _string)
{ return send_to_stream(_string) ;}

log_client& log_client::operator<<(std::string* _string_ptr)
{ return send_to_stream(_string_ptr) ;}

log_client& log_client::operator<< (std::streambuf* _streambuf_ptr)
{ return send_to_stream(_streambuf_ptr) ;}

log_client& log_client::operator<< (std::ostream& ( *pf )(std::ostream&))
{ return send_to_stream(pf) ;}

log_client& log_client::operator<< (std::ios& ( *pf )(std::ios&))
{ return send_to_stream(pf) ;}

log_client& log_client::operator<< (std::ios_base& ( *pf )(std::ios_base&))
{ return send_to_stream(pf) ;}
////////////////////////////////////////////////////////////////////////////////////////
// end of the operator overload block
//

} // namespace inglenook::logging

} // namespace inglenook
