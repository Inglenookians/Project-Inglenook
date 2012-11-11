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

// inglenook includes
#include "log_write_actions.h"

namespace inglenook
{

namespace logging
{

/**
 * Parses an action string and determines the action it relates to.
 * @params action_string action string to parse.
 * @returns action to perform, or no_action on failure.
 */
log_write_action parse_action(const std::string& action_string)
{

	// initialize result buffer
	auto result = log_write_action::no_action;
	
	// determine what is being requested
	if		(action_string == start_full_action_string) 	result = create_new_log;
	else if	(action_string == start_short_action_string) 	result = create_new_log;
	else if	(action_string == write_full_action_string) 	result = write_message_to_log;
	else if	(action_string == write_short_action_string) 	result = write_message_to_log;
	else if	(action_string == close_full_action_string) 	result = close_log;
	else if	(action_string == close_short_action_string) 	result = close_log;
	
	// return the result
	return result;
	
}

} // namespace logging

} // namespace inglenook