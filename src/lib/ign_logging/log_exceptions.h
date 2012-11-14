/*
* log_exceptions.h: LogWriter exception constants and structures..
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
#include <exception>

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>
#include <boost/locale.hpp>

// inglenook includes
#include <ign_core/exceptions.h>

namespace inglenook
{

namespace logging
{

/// base of all logging exceptions
const unsigned long module_error_base = 0xff00000;

/// bad log output stream in log_writer::log_writer(std::ostream&)
const unsigned long log_exception_bad_stream = module_error_base + 0x01;

/// bad file path provided for log_writer::create_from_file_path(path, [create])
const unsigned long log_exception_bad_file_path = module_error_base + 0x02;

/// used when a thread attempts to acquire the shutdown lock for the writer thread, but fails.
const unsigned long unable_to_aquire_shutdown_lock = module_error_base + 0x03;

/// used by the serialization thread when it cannot acquire ownership of its notification mechanism lock.
const unsigned long unable_to_aquire_queue_notification_lock = module_error_base + 0x04;

/// log file that was being written to (or attempted writing to) at time of exception.
typedef boost::error_info<struct __log_file_name, boost::filesystem::path> log_file_name;

//
// log_exception
// Standard base exception for all exceptions thrown by objects in the logging library.
struct log_exception: virtual inglenook::core::exceptions::inglenook_exception
{/// provides a boiler plate explanation of the exception.
   const char* what() const throw() {
	   return boost::locale::translate("Unhandled exception in inglenook logging mechanism").str().c_str();
   }
};

//
// log_not_found_exception
// This exception is thrown when a specified log file cannot be found and, when
// available, the option to create non-existent log files was disabled.
struct log_not_found_exception : virtual log_exception
{/// provides a boiler plate explanation of the exception.
   const char* what() const throw() {
	   return boost::locale::translate("Failed to open the specified log file.").str().c_str();
   }
};

//
// failed_to_create_log_exception
// This exception is thrown when Inglenook fails to create an error log.
struct failed_to_create_log_exception : virtual log_exception
{  /// provides a boiler plate explanation of the exception.
   const char* what() const throw() {
	   return boost::locale::translate("Failed to create the specified log file.").str().c_str();
   }
};

//
// serialization_exception
// This exception is thrown by the log serialization worker (log_writer::_log_serialization_worker).
struct log_serialization_exception : virtual log_exception
{	/// provides a boiler plate explanation of the exception.
   const char* what() const throw() {
	   return boost::locale::translate("There was a problem with the log serialization mechanism").str().c_str();
   }
};

} // namespace inglenook::logging

} // namespace inglenook
