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

// boost (http://boost.org) includes
#include <boost/locale.hpp>

// inglenook includes
#include <ign_core/application.h>
#include <ign_logging/logging.h>
#include "log_write_exceptions.h"
#include "version.h"

bool handle_program_options()
{

    // Create program options for log creation...
    boost::program_options::options_description creation_options("Log creation options");
    creation_options.add_options()
		("start,s", "Starts a new log file.")
		("name,n", boost::program_options::value<std::string>()->required(), "Name of script or process.")
		("pid,p", boost::program_options::value<int>()->required(), "Process id of script or process.");

    // Create program options for log writing...
    boost::program_options::options_description writing_options("Log writing options");
    writing_options.add_options()
		("write,w", "Write an entry to the log file.")
		("filename,f", boost::program_options::value<std::string>()->required(), "Path to log file.")
		("message,m", boost::program_options::value<int>()->required(), "Process id of script or process.");

    // Create program options for closing log files...
    boost::program_options::options_description termination_options("Log termination options");
    writing_options.add_options()
		("close,c", "Closes the log file.")
		("filename,f", boost::program_options::value<std::string>()->required(), "Path to log file.");

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
	
	try
	{
		
	    // set the program description.
	    std::string description(boost::locale::translate("A command line utility for writing Inglenook log files."));
	
	    // Create the application store.
	    inglenook::core::application app(description, VERSION,  __DATE__, __TIME__);

	    if(handle_program_options())
	    {

	    }
	    
		// the binary has (for all we know) done its job.
		success = EXIT_SUCCESS;
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
