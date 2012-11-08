/*
 * log_entry.h: Represents a single log entry.
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
#include <string>
#include <map>

namespace inglenook
{

namespace logging
{

/**
 * Log entry types
 * Used to categorize log message semantics. Each category is assigned a value related to
 * its expected verbosity and significance. The lower the value, the more verbose the
 * category is expected to be. The exception to this rule is category::unspecified which
 * if dynamically remapped to log_client::default_category.
 */
enum category : unsigned int
{
	unspecified = 0x00,  	/**< Use category defined by log_client::default_category. */
	debugging 	= 0x01,		/**< Debugging information. Extremely verbose, usually only beneficial during development.*/
	verbose 	= 0x02, 	/**< Verbose output. Useful for diagnosing hard to track issues in live environments. */
	information = 0x03,		/**< Information. Standard output messages related to key events within the software. */
	warning 	= 0x04,		/**< Warning. Notification of potentially erroneous or unexpected behavior. Usually not fatal . */
	error 		= 0x05,		/**< Error. Significant deviation of expected behavior. */
	fatal		= 0x06,		/**< Fatal Error. Unrecoverable deviation from expected behaviour. This will usually be the last entry. */
	no_log		= 0x99		/**< No logging. (log_writer use only). */
};

/**
 * Log entry
 * Contains all the appropriate information for a single log entry. Created by log_clients, and passed through to
 * log_writer for serialization. These are built up in the background by inglenook::log::log_client in the background
 * when log_client is used with a stream operator.
 */
class log_entry
{

	public:

		// creates a new log entry
		log_entry();

		// deletes an existing log entry.
		virtual ~log_entry();


		/// gets the current category
		const category& entry_type() const;

		/// sets the current category
		void entry_type(const category& value);

		/// gets the log name space
		const std::string& log_namespace() const;

		/// sets the log name space
		void log_namespace(const std::string& value);

		/// gets the log message
		virtual const std::string& message();

		/// sets the log message
		virtual void message(const std::string& value);

		/// add a data entry to the log
		void extended_data(const std::string& key, const std::string& value);

		/// get the data records from the log.
		const std::map<std::string, std::string>& extended_data();

	private:

		/// internal variable for category.
		category m_category = category::unspecified;

		/// name space for this message
		std::string m_namespace;

		/// logs message body
		std::string m_message;

		/// buffer for extended data.
		std::map<std::string, std::string> m_extended;

};

} // namespace inglenook::logging

} // namespace inglenook
