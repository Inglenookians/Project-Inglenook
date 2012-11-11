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
	log_output = log_writer::create_from_file_path( (log_file) );
	ilog = std::shared_ptr<log_client>(new log_client(log_output));
}

} // namespace logging


} // namespace inglenook
