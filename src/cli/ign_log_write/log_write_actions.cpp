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

// boost (http://boost.org) includes
#include <boost/locale.hpp>

// inglenook includes
#include <ign_core/application.h>
#include "log_write_actions.h"
#include "log_write_exceptions.h"

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

/**
 * extracts a required parameter from the arguments vector.
 * @params from arguments vector to extract value from.
 * @params parameter parameter to extract.
 * @throws action_required_arguments_missing if the required argument is missing.
 * @returns value of the required argument
 */
template <class T> T require_parameter(const boost::program_options::variables_map& from, const std::string& parameter)
{

	// ensure the value is present...
	if(from.count(parameter) == 0)
	{
		using namespace inglenook::logging;

		// throw exception - item is missing...
		BOOST_THROW_EXCEPTION( action_required_arguments_missing_exception()
				<< expected_argument(parameter));
	}

	// return the item required.
	return from[parameter].as<T>();
}


/**
 * extracts an optional parameter from the arguments vector.
 * @params from arguments vector to extract value from.
 * @params parameter parameter to extract.
 * @params fallback what to use if the parameter isn't present
 * @returns value of the optional argument
 */
template <class T> T optional_parameter(const boost::program_options::variables_map& from, const std::string& parameter, const T& fallback)
{

	// create and default buffer
	T result = fallback;

	// check if the value is present...
	if(from.count(parameter) != 0)
	{
		// ... and populate result if so
		result = from[parameter].as<T>();
	}

	// return the item.
	return result;
}


/**
 * Converts an unsigned integer in to a category
 * @params value value to convert.
 * @returns value as category, or information on failure.
 */
category convert_to_category(unsigned int value)
{

	// preload buffer with the default
	auto result = category::information;

	// check for deviation
	switch(value)
	{
		case 1: // debugging category
			result = category::debugging;
			break;

		case 2:  // verbose category
			result = category::verbose;
			break;

		case 4:  // warning category
			result = category::warning;
			break;

		case 5:  // error category
			result = category::error;
			break;

		case 6:  // fatal category
			result = category::fatal;
			break;
	}

	return result;

}

/**
 * Creates a new file using the arguments provided.
 * @params arguments command line arguments to extract log creation parameters from.
 * @returns path to created file on success; or an empty path on failure.
 */
const boost::filesystem::path create_log_file(const boost::program_options::variables_map& arguments)
{

	using namespace boost::locale;

	boost::filesystem::path result = "";

	// based on the size of the arguments vector - choose how to process
	// the create request
	switch (arguments.size())
	{
		case 2: // expect create action and a file name.
		{
			// extract the log name and call the appropriate method...
			auto log_file = require_parameter<std::string>(arguments, "filename");
			result = create_log_file(log_file);
			break;
		}
		case 3: // expect create action, a process id and name.
		{
			// extract the pid and name, then cann the appropriate method...
			auto pid = require_parameter<pid_type>(arguments, "pid");
			auto name = require_parameter<std::string>(arguments, "name");
			result = create_log_file(name, pid);
			break;
		}
		default: // improper number of arguments - tell user whats wrong.
		{

			// build the explaination string
			std::string acceptable_arguments_string = translate("The create action can take either one argument, the log file "
					"you wish to create (-f), or two arguments, the process id (-p) and name of the program (-n) you "
					"are logging on behalf of");

			// throw exception - tell the user what we expected
			BOOST_THROW_EXCEPTION( wrong_number_of_arguments_exception()
					<< acceptable_arguments(acceptable_arguments_string));

			break;
		}

	}

	// return the result
	return result;
}


/**
 * Creates a new file using the specified log path
 * @params path_to_log path to the log file to create
 * @returns path to created file on success; or an empty path on failure.
 */
const boost::filesystem::path create_log_file(const boost::filesystem::path& path_to_log)
{
	using namespace inglenook::logging;
	using namespace boost::locale;

	// make things a little more readable
	const bool create_file_if_not_exists = true;
	const bool emmit_xml_header = true;
	const bool emmit_xml_footer = false;

	// create the log file and a log client to work with
	auto _log_writer = log_writer::create_from_file_path(path_to_log,
			create_file_if_not_exists, emmit_xml_header, emmit_xml_footer);
	log_client _log_client(_log_writer);

	// we want to only have the name of the log file printed out to console. but we want to put
	// some content in the XML file (so we know when the script started running if the user script
	// doesn't do this itself). As such, turn off console logging.
	_log_writer->console_threshold(category::no_log);

	// write out that we started logging (marks the start of the running script if they do not emmit there own note).
	_log_client.info() << ns(log_write_default_namespace) << translate("Started new logging session.") << lf::end;

	// return the new log file.
	return path_to_log;
}

/**
 * Creates a new file using the name and process id specified
 * @params name specific name to log for.
 * @params pid specific pid to log for.
 * @returns path to created file on success; or an empty path on failure.
 */
const boost::filesystem::path create_log_file(const std::string& name, const pid_type& pid)
{
	// determine where to log to
	auto log_file = log_writer::default_log_path(pid, name);

	// write out the new file
	return create_log_file(log_file);

}

/**
 * Writes a log message with information extracted from the arguments collection.
 * @params arguments arguments to extract message from.
 */
void write_log_entry(const boost::program_options::variables_map& arguments)
{
	// create defaults - we only actually require the log path and the message
	std::string file_path 		= require_parameter	<std::string>(arguments, "filename");
	std::string message 		= require_parameter	<std::string>(arguments, "message");;
	std::string log_namespace 	= optional_parameter<std::string>(arguments, "namespace", log_write_default_namespace);
	unsigned int event_type 	= optional_parameter<unsigned int>(arguments, "category", category::information);

	// write out the log entry
	write_log_entry(file_path, message, log_namespace, convert_to_category(event_type) );

}

/**
 * Writes a log message with the specified information
 * @params path_to_log log file to write out to.
 * @params message message to write out.
 * @params log_namespace namespace to write the log out to
 * @params category the type of event to write
 */
void write_log_entry(const boost::filesystem::path& path_to_log, std::string message, std::string log_namespace, category event_type)
{
	using namespace inglenook::logging;
	using namespace boost::locale;

	// make things a little more readable
	const bool create_file_if_not_exists = false;
	const bool emmit_xml_header = false;
	const bool emmit_xml_footer = false;

	// create the log file and a log client to work with
	auto _log_writer = log_writer::create_from_file_path(path_to_log,
		create_file_if_not_exists, emmit_xml_header, emmit_xml_footer);
	log_client _log_client(_log_writer);

	_log_client.buffer()->entry_type(event_type);

	// write out that we started logging (marks the start of the running script if they do not emmit there own note).
	_log_client << ns(log_namespace) << message << lf::end;

}


} // namespace logging

} // namespace inglenook
