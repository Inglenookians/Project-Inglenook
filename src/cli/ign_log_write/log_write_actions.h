/*
* log_write_actions.h/cpp: ign_log_write functions and definitions based on actions.
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

// boost (http://boost.org) includes
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

// inglenook includes
#include <ign_logging/logging.h>

namespace inglenook
{

namespace logging
{

// start log file action strings
const std::string start_full_action_string = "start";
const std::string start_short_action_string = "s";

// write log entry strings
const std::string write_full_action_string = "write";
const std::string write_short_action_string = "w";

// close log file action strings
const std::string close_full_action_string = "close";
const std::string close_short_action_string = "c";

/**
 * Log writer action
 * indicates what action should be performed by the log writer.
 */
enum log_write_action : unsigned int
{
	no_action 				= 0x00, /**< no action to be performed. */
	create_new_log 			= 0x01, /**< create a new log file. */
	write_message_to_log 	= 0x02,	/**< write a message to an existing log file. */
	close_log 				= 0x03  /**< close an open log file. */
};

/// default namespace to write entries in when using this binary.
const std::string log_write_default_namespace = "inglenook.logging";

// parses an action string and returns the corresponding action
log_write_action parse_action(const std::string& action_string);

// extracts a parameter from the specified map, throwing an exception on failure
template <class T> T require_parameter(const boost::program_options::variables_map& from, const std::string& parameter);

// extracts a parameter from the specified map, returning a fallback on failure
template <class T> T optional_parameter(const boost::program_options::variables_map& from, const std::string& parameter, const T& fallback);

// converts an unsigned int in to a category
category convert_to_category(unsigned int value);



// creates a new log file with using the arguments provided.
const boost::filesystem::path create_log_file(const boost::program_options::variables_map& arguments);

// creates a new log file using the specified file name.
const boost::filesystem::path create_log_file(const boost::filesystem::path& path_to_log);

// creates a new log file using the specified PID and name
const boost::filesystem::path create_log_file(const std::string& name, const pid_type& pid);



// writes a log entry using the arguments provided.
void write_log_entry(const boost::program_options::variables_map& arguments);

// writes a log entry to the specified file with the specified message, namespace and category.
void write_log_entry(const boost::filesystem::path& path_to_log, std::string message, std::string log_namespace = log_write_default_namespace, category event_type = category::information);



// closes the log file identified in the program arguments.
void close_log_file(const boost::program_options::variables_map& arguments);

// closes the specified log file.
void close_log_file(const boost::filesystem::path& path_to_log);


} // namespace logging

} // namespace inglenook
