/*
 * log_entry.cpp: Represents a single log entry.
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

// boost includes
#include <boost/exception/all.hpp>

// inglenook includes
#include "log_exceptions.h"
#include "log_entry.h"

namespace inglenook
{

namespace logging
{

/**
* Creates a new log_entry, initializing all members to default values.
* @see ~log_entry()
*/
log_entry::log_entry()
:   m_namespace(""),
    m_message("")

{
	this->m_extended.clear();
}

/**
* Deconstructs the log_entry, releasing any associated resources.
* @see log_entry()
*/
log_entry::~log_entry()
{
	// nothing to do here.
}

/**
 * Gets the entry category type.
 * This method gets the current log category, which is used for determining what type
 * of entry this is (usually relating to the severity of the message). The writer will
 * use this information to filter how content is processed, if it is at all.
 * @returns current entry category type.
 */
const category& log_entry::entry_type() const
{
	return m_category;
}

/**
 * Sets the entry category type.
 * This method sets the current log category, which is used for determining what type
 * of entry this is (usually relating to the severity of the message). The writer will
 * use this information to filter how content is processed, if it is at all.
 * @param value new value for the property.
 */
void log_entry::entry_type(const category& value)
{
	m_category = value;
}

/**
 * Gets the entry name space.
 * This method gets the current log name space, which is designed to allow logs to be
 * grouped in to functional area's (for example; to identify the subsystem raising an
 * entry). This information can be useful to reduce log entries that are unrelated to
 * a users interest in the logs and help highlight relevant information in large logs.
 * @returns current entry name space.
 */
const std::string& log_entry::log_namespace() const
{
	return m_namespace;
}

/**
 * Sets the entry name space.
 * This method sets the current log name space, which is designed to allow logs to be
 * grouped in to functional area's (for example; to identify the subsystem raising an
 * entry). This information can be useful to reduce log entries that are unrelated to
 * a users interest in the logs and help highlight relevant information in large logs.
 * @param value properties new value.
 */
void log_entry::log_namespace(const std::string& value)
{
	m_namespace = value;
}

/**
 * Sets the entry message.
 * This method sets the current log message body, This is the core part of the log and
 * the only required field (where other parts are technically required, they will be
 * automatically completed with default values by logging mechanisms if neglected).
 * @param value new property value.
 */
void log_entry::message(const std::string& value)
{
	m_message = value;
}

/**
 * Gets the entry message.
 * This method gets the current log message body, This is the core part of the log and
 * the only required field (where other parts are technically required, they will be
 * automatically completed with default values by logging mechanisms if neglected).
 * @returns current entry message.
 */
const std::string& log_entry::message()
{
	return m_message;
}

/**
 * Adds or amends extended data associated with the entry.
 * This method is used to supplement the body of the log with potentially useful information.
 * Data is added as key/value pairs and should be attached to the log as some form of table.
 * @param key data element identifier (if the value already exists modifies existing item).
 * @param value data element value (if empty string will attempt to remove existing key).
 */
void log_entry::extended_data(const std::string& key, const std::string& value)
{
	// ensure we are actually adding something
	if(value.length() > 0)
	{
		// the [] operator does exactly what we want (creates or updates key based
		// on prior existence, so just rely on this behavior.
		m_extended[key] = value;
	}
	// else quickly check if we need to remove something.
	else
	{
		// attempt to find the key in the collection
		auto iterator = m_extended.find(key);

		// and on success remove it.
		if(iterator != m_extended.end())
			m_extended.erase(iterator);
	}
}

/**
 * Gets the extended data associated with the entry.
 * This data is used to supplement the body of the log with potentially useful information.
 * Data is added as key/value pairs and should be attached to the log as some form of table.
 * @returns entries extended data.
 */
const std::map<std::string, std::string>& log_entry::extended_data()
{
	return m_extended;
}





} // namespace inglenook::logging

} // namespace inglenook
