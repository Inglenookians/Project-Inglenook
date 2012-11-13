/*
* logging.cpp: Provides initialization and other basic functionality
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
#include "logging.h"

namespace inglenook
{

namespace logging
{

/**
 * Initializes the logging system using default parameters
 * This is the easiest way to get logging initialized; it will create a writer that will output
 * to both console, and a new log file - the path of which is determined by log_writer::default_log_path()
 */
void initialize_logging()
{
	// initialize logging functionality
	initialize_logging(log_writer::default_log_path());
}

/**
 * Initializes the logging system using a specific file path
 * Initializes logging, emmiting log entries to the specified log file.
 */
void initialize_logging(const boost::filesystem::path& log_file)
{
	// create global variables and output appropriately.
	log_output = log_writer::create_from_file_path( log_file );
	ilog = std::shared_ptr<log_client>(new log_client(log_output));
}

/**
 * Initializes the logging system as "off-the-record"
 * This intialization method creates a new logging interface that does not write permanent files.
 */
void initialize_logging_off_record()
{
	// create global variables and output appropriately.
	log_output = log_writer::create_from_stream( nullptr );
	ilog = std::shared_ptr<log_client>(new log_client(log_output));
}

/**
 * Checks if the logging system is initialized and provides just-in-time initialization.
 * @params warning indicates if the fallback warning should be shown if just-in-time initialization was required.
 */
void logging_initialization_check(const bool& warning)
{

	// check if logging is properly instanciated.
	if(log_output == nullptr || ilog == nullptr)
	{
		// create fallback logger - do not assume we have disk access.
		initialize_logging_off_record();

		// check if this fail-safe is silent.
		if(warning)
		{
			// warn implementors that an automatic check just saved them. if you want basic logging no file - call  initialize_logging_off_record() yourself.
			log_warning() << boost::locale::translate("WARNING: use of inglenook logging prior to initialization. an off-the-record logging mechanism has been") << std::endl;
			log_warning() << boost::locale::translate("         initialized on your behalf, its strongly recommended you do this yourself.") << lf::end;
		}

	}

}

/**
 * Sets the specified category on the log stream and returns the default log interface.
 * @param new_value new category to assign the writer.
 * @returns default logging interface.
 */
log_client& set_category_and_return(const category& new_value)
{
	logging_initialization_check();
	ilog->buffer()->entry_type(new_value);
	return *ilog;
}

/**
 * Continues an existing log entry.
 */
log_client& log()
{
	logging_initialization_check();
	return *ilog;
}

/**
 * Continues an existing log entry, switching context to debug.
 */
log_client& log_debug()
{
	return set_category_and_return(category::debugging);
}

/**
 * Continues an existing log entry, switching context to trace.
 */
log_client& log_trace()
{
	return set_category_and_return(category::verbose);
}

/**
 * Continues an existing log entry, switching context to information.
 */
log_client& log_info()
{
	return set_category_and_return(category::information);
}

/**
 * Continues an existing log entry, switching context to warning.
 */
log_client& log_warning()
{
	return set_category_and_return(category::warning);
}

/**
 * Continues an existing log entry, switching context to error.
 */
log_client& log_error()
{
	return set_category_and_return(category::error);
}

/**
 * Continues an existing log entry, switching context to fatal.
 */
log_client& log_fatal()
{
	return set_category_and_return(category::fatal);
}

} // namespace logging

} // namespace inglenook

