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

// parses an action string and returns the corresponding action
log_write_action parse_action(const std::string& action_string);

//void create_log_file(boost::program_options arguments);

} // namespace logging

} // namespace inglenook
