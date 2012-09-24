/*
* log_entry_buffer.h: data buffer used in tls in log_client.
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

#ifndef LOG_ENTRY_BUFFER_H_
#define LOG_ENTRY_BUFFER_H_

// standard library includes.
#include <sstream>

// inglenook includes
#include "log_entry.h"

namespace inglenook
{

namespace logging
{
/**
 * Data type used to buffer log parameters.
 * The log_entry_buffer class is the data type used in log_client thread local storage (tls).
 * either via the add_entry() method call, or the stream operator.
 */
class log_entry_buffer
{
public:

	/// creates a new entry buffer and initializes appropraite resources.
	log_entry_buffer()
		: message(std::shared_ptr<std::stringstream>(new std::stringstream()))
	{
		// nothing to do - we just need to initialize the message stream (done above).
	}

	/// name space that the entry should be created under.
	std::string log_namespace;

	/// message to be sent to the message stream.
	std::shared_ptr<std::stringstream> message;

	/// category to send log entry to.
	category entry_type = category::unspecified;

};

} // namespace inglenook::logging

} // namespace inglenook

#endif /* LOG_ENTRY_BUFFER_H_ */
