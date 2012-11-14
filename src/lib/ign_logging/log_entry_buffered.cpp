/*
 * log_entry_unbuffered.cpp: Represents a single, buffered, log entry.
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
#include "log_entry_buffered.h"

namespace inglenook
{

namespace logging
{

/**
* Creates a new log_entry_buffered, initializing all members to default values.
* @see ~log_entry_buffered()
*/
log_entry_buffered::log_entry_buffered()
	: log_entry(),
	m_message_buffer()
{
	// Nothing to do here at the moment.
}

/**
* Deconstructs the log_entry_buffered, releasing any associated resources.
* @see log_entry_buffered()
*/
log_entry_buffered::~log_entry_buffered()
{
	// Nothing to do here at the moment.
}


/**
 * Gets the entry's message buffer
 * This method returns the log_entries message buffer so that the message can be manipulated.
 * @returns entry message buffer.
 */
std::stringstream& log_entry_buffered::message_buffer()
{
	return m_message_buffer;
}

/**
 * Gets the entry message.
 * This method gets the current log message body, This is the core part of the log and
 * the only required field (where other parts are technically required, they will be
 * automatically completed with default values by logging mechanisms if neglected).
 * @returns current entry message.
 */
const std::string& log_entry_buffered::message()
{
	// update the underlying buffer, and return as if a normal log_entry
	log_entry::message(std::string(m_message_buffer.str()));
	return log_entry::message();
}

/**
 * Sets the entry message.
 * This method sets the current log message body, This is the core part of the log and
 * the only required field (where other parts are technically required, they will be
 * automatically completed with default values by logging mechanisms if neglected).
 * @param value new property value.
 */
void log_entry_buffered::message(const std::string& value)
{
	// reset string stream
	m_message_buffer.clear();
	m_message_buffer.str(std::string());

	// update with new value
	m_message_buffer << value;
	log_entry::message(value);
}


} // namespace inglenook::logging

} // namespace inglenook
