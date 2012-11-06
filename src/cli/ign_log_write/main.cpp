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
#include "log_write_exceptions.h"

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