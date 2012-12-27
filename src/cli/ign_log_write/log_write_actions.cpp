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
#include "program_options.h"

namespace inglenook
{

namespace logging
{

/**
 * Parses an action string and determines the action it relates to.
 * @param action_string action string to parse.
 * @returns action to perform, or no_action on failure.
 */
log_write_action parse_action(const std::string& action_string)
{
    // initialize result buffer
    auto result = log_write_action::no_action;

    //
    // determine what is being requested
    // and assign an appropriate value
    //

    if(action_string == start_full_action_string)
    {
        result = create_new_log;
    }
    else if(action_string == start_short_action_string)
    {
        result = create_new_log;
    }
    else if(action_string == write_full_action_string)
    {
        result = write_message_to_log;
    }
    else if(action_string == write_short_action_string)
    {
        result = write_message_to_log;
    }
    else if(action_string == close_full_action_string)
    {
        result = close_log;
    }
    else if(action_string == close_short_action_string)
    {
        result = close_log;
    }
    
    // return the result
    return result;
}



/**
 * Converts an unsigned integer in to a category
 * This method is designed to ensure that no values are used which are outside the range supported by the category enumeration
 * @param value value to convert.
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
 * @param	 arguments command line arguments to extract log creation parameters from.
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
            auto log_file = require_parameter<std::string>(arguments, PO_FILENAME_FULL);
            result = create_log_file(log_file);
            break;
        }
        case 3: // expect create action, a process id and name.
        {
            // extract the pid and name, then call the appropriate method...
            auto pid = require_parameter<pid_type>(arguments, PO_PID_FULL);
            auto name = require_parameter<std::string>(arguments, PO_NAME_FULL);
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
 * @param path_to_log path to the log file to create
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
 * @param name specific name to log for.
 * @param pid specific pid to log for.
 * @returns path to created file on success; or an empty path on failure.
 */
const boost::filesystem::path create_log_file(const std::string& name, const pid_type& pid)
{
    // determine where to log to
    auto log_file = log_writer::default_log_path(pid, name);

    using namespace inglenook::logging;
    using namespace boost::locale;

    // make things a little more readable
    const bool emmit_xml_header = true;
    const bool emmit_xml_footer = false;

    auto path_to_log = std::shared_ptr<boost::filesystem::path>(new boost::filesystem::path());

    // create the log file and a log client to work with
    auto _log_writer = log_writer::create(emmit_xml_header, emmit_xml_footer, pid, name, path_to_log);
    log_client _log_client(_log_writer);

    // we want to only have the name of the log file printed out to console. but we want to put
    // some content in the XML file (so we know when the script started running if the user script
    // doesn't do this itself). As such, turn off console logging.
    _log_writer->console_threshold(category::no_log);

    // write out that we started logging (marks the start of the running script if they do not emmit there own note).
    _log_client.info() << ns(log_write_default_namespace) << translate("Started new logging session.") << lf::end;

    // return the new log file.
    return boost::filesystem::path( boost::filesystem::system_complete(*path_to_log) );

}

/**
 * Writes a log message with information extracted from the arguments collection.
 * @param arguments arguments to extract message from.
 */
void write_log_entry(const boost::program_options::variables_map& arguments)
{
    // create defaults - we only actually require the log path and the message
    std::string file_path       = require_parameter <std::string>(arguments, PO_FILENAME_FULL);
    std::string message         = require_parameter <std::string>(arguments, PO_MESSAGE_FULL);
    std::string log_namespace   = optional_parameter<std::string>(arguments, PO_NAMESPACE_FULL, log_write_default_namespace);
    unsigned int event_type     = optional_parameter<unsigned int>(arguments, PO_CATEGORY_FULL, category::information);

    // write out the log entry
    write_log_entry(file_path, message, log_namespace, convert_to_category(event_type) );
}

/**
 * Writes a log message with the specified information
 * @param path_to_log log file to write out to.
 * @param message message to write out.
 * @param log_namespace namespace to write the log out to
 * @param event_type the type of event to write
 */
void write_log_entry(const boost::filesystem::path& path_to_log, const std::string& message, const std::string& log_namespace, const category& event_type)
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


/**
 * Closes the log file idenfied by the arguments.
 * @param arguments arguments to extract file name from.
 */
void close_log_file(const boost::program_options::variables_map& arguments)
{
    // extract the log name and call close method...
    auto log_file = require_parameter<std::string>(arguments, PO_FILENAME_FULL);
    close_log_file(log_file);
}

/**
 * Closes the specified log file.
 * @param path_to_log log file to close.
 */
void close_log_file(const boost::filesystem::path& path_to_log)
{
    using namespace inglenook::logging;
    using namespace boost::locale;

    // make things a little more readable
    const bool create_file_if_not_exists = false;
    const bool emmit_xml_header = false;
    const bool emmit_xml_footer = true;

    // create the log file and a log client to work with
    auto _log_writer = log_writer::create_from_file_path(path_to_log,
            create_file_if_not_exists, emmit_xml_header, emmit_xml_footer);
    log_client _log_client(_log_writer);

    // No console output is expected from this method. but we want to put some content in
    // the XML file (so we know when the script has finished running if the user script
    // doesn't do this itself). As such, turn off console logging.
    _log_writer->console_threshold(category::no_log);

    // write out that we started logging (marks the start of the running script if they do not emmit there own note).
    _log_client.info() << ns(log_write_default_namespace) << translate("Terminated logging session.") << lf::end;
}

} // namespace logging

} // namespace inglenook
