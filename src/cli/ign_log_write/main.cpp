/*
 * main.cpp: CLI tool for writing log files.
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

// standard library includes
#include <stdlib.h>
#include <sstream>

// boost (http://boost.org) includes
#include <boost/locale.hpp>

// inglenook includes
#include <ign_core/application.h>
#include <ign_core/application_exceptions.h>
#include <ign_logging/logging.h>
#include "log_write_exceptions.h"
#include "log_write_actions.h"
#include "version.h"

/**
 * Creates the programs arguments structure for the software.
 * @returns program options descriptor.
 */
boost::program_options::options_description create_program_options()
{

	using namespace inglenook::logging;
	using namespace boost::locale;

	std::stringstream actions_description;

	// create the description for the action group
	actions_description << translate("REQUIRED: Indicates the type of action to perform:\n\n")
			<< start_short_action_string << ", [ "<< start_full_action_string << translate(" ] \tStarts a new log file.\n")
			<< write_short_action_string << ", [ "<< write_full_action_string << translate(" ] \tWrite an entry to the specified log file.\n")
			<< close_short_action_string << ", [ "<< close_full_action_string << translate(" ] \tCloses the specific log file.");

	// create the actions group
	boost::program_options::options_description action_options( translate("Actions") );
	action_options.add_options()
		("action,a",  	boost::program_options::value<std::string>()->required(), 	actions_description.str().c_str())
		("filename,f", 	boost::program_options::value<std::string>(), 				translate("Path to log file.").str().c_str());

    // Create program options for log creation...
    boost::program_options::options_description creation_options(translate("Log creation specific options (s, start)"));
    creation_options.add_options()
		("name,n", 		boost::program_options::value<std::string>(), 				translate("REQUIRED: Name of script or process.").str().c_str())
		("pid,p", 		boost::program_options::value<int>(), 						translate("REQUIRED: Process id of script or process.").str().c_str());

    // Create program options for log writing...
    boost::program_options::options_description writing_options( translate("Log writing specific options (w, write)") );
    writing_options.add_options()

		("message,m", 	boost::program_options::value<std::string>(), 				translate("REQUIRED: Process id of script or process.").str().c_str());


    // add all the options
    boost::program_options::options_description all_options( translate("All available options") );
    all_options.add(action_options).add(creation_options).add(writing_options);

    return all_options;
}


/**
 * Creates the programs positional arguments descriptor.
 * @returns program positional arguments descriptor.
 */
boost::program_options::positional_options_description create_program_positionals()
{
    // Set the positional options.
    boost::program_options::positional_options_description positions;
    positions.add("action", 1);
    positions.add("filename", 1);
    positions.add("message", 1);
    return positions;
}

/**
 * extracts a required parameter from the arguments vector.
 * @params from arguments vector to extract value from.
 * @params parameter parameter to extract.
 * @throws action_required_arguments_missing if the required argument is missing.
 * @returns value of the required argument
 */
template <class T> T require_parameter(const boost::program_options::variables_map& from, const std::string parameter)
{

	// ensure the value is present...
	if(from.count(parameter) == 0)
	{
		using namespace inglenook::logging;

		// throw exception - item is missing...
		BOOST_THROW_EXCEPTION( action_required_arguments_missing()
				<< expected_argument(parameter));
	}

	// return the item required.
	return from[parameter].as<T>();
}

/**
 * This is the main entry point of the ign_log_write application.
 * @params arg_c number of command line arguments
 * @params arg_v list of command line arguments
 * @returns EXIT_SUCCESS on success, EXIT_FAILURE on generic failure.
 */
int main(int arg_c, char* arg_v[])
{
	
	int success = EXIT_FAILURE;
	
	using namespace boost::locale;
	using namespace inglenook::logging;

	try
	{
		
		// ign_log_write does not use logging conventionally - skipping logging initialization.

	    // set the core program variables for naming, versioning and support.
	    std::string description(boost::locale::translate("command line utility for writing Inglenook log files"));
	    inglenook::core::application app(description, VERSION,  __DATE__, __TIME__);

	    // result buffer for command line parsing...
	    boost::program_options::variables_map vm;

	    // attempt to parse the command line
	    if(!inglenook::core::application::arguments_parser(vm, arg_c, arg_v,
	    		create_program_options(), create_program_positionals()))
	    {

	    	std::string action ( vm["action"].as<std::string>() );

	    	// determine what was asked for...
	    	switch( parse_action(action) )
	    	{
	    		// caller wants to create a new log
	    		case log_write_action::create_new_log:
	    		{
	    			std::cout << "log_write_action::create_new_log" << std::endl;
	    			break;
	    		}
	    		// caller wants to write to an existing log
	    		case log_write_action::write_message_to_log:
				{

					// extract required paramaters
					std::string log_path 	= require_parameter<std::string>(vm, "filename");
					std::string message 	= require_parameter<std::string>(vm, "message");

					std::cout << "writing " << message << " to '" << log_path << "'"<< std::endl;
					break;
				}
				// caller wants to close an open log file.
	    		case log_write_action::close_log:
				{
					// extract required paramaters
					std::string log_path 	= require_parameter<std::string>(vm, "filename");

					std::cout << "closing log file '" << log_path << "'"<< std::endl;
					break;
				}
				// we have no idea what the caller wants to do.
	    		default:
	    		{
	    			std::cout << "default" << std::endl;
	    			break;
	    		}
	    	}

	    }
	    
		// the binary has (for all we know) done its job.
		success = EXIT_SUCCESS;
	}
	catch(inglenook::logging::action_required_arguments_missing& ex)
	{

		// tell the user what went wrong...
		if( const std::string* missing_argument = boost::get_error_info<expected_argument>(ex) )
			 std::cerr << translate("ERROR: The specified action requires an additional argument: ") << *missing_argument << std::endl;
		else std::cerr << translate("ERROR: The specified action requires an unspecified additional argument") << std::endl;

		// be helpful - suggest where the user can get additional support using this tool
		std::cerr << translate("For support with this library check the manual documentation by typing \"man ") << inglenook::core::application::name() << "\"" << std::endl;

	}
	catch(inglenook::core::exceptions::application_arguments_parser_exception&)
	{
		/* nothing to do - arguments passed to the program were incorrect */

		// be helpful - suggest where the user can get additional support using this tool
		std::cerr << translate("For support with this library check the manual documentation by typing \"man ") << inglenook::core::application::name() << "\"" << std::endl;

	}
	catch(...) // this is the "its gone seriously wrong" error handler.
	{
		// somethings gone awry and we've not caught it at all. Usually this exception would be written
		// to ilog.fatal(), but as this binary is part of the logging system, this would probably be a "bad idea(tm)".
        std::cerr << boost::locale::translate("Unhandled exception in ign_log_write:") << std::endl <<
            boost::current_exception_diagnostic_information();	
	}
	
	return success;
	
}
