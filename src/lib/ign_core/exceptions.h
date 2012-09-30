#pragma once

/*
 * exceptions.h: Defines common inglenook exception types.
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
#include <exception>

// boost (http://boost.org) includes
#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

namespace inglenook
{
    namespace core
    {
        namespace exceptions
        {
            
            /// Error number used to refine error site when stack traces are not available.
            typedef boost::error_info<struct __inglenook_error_number, unsigned long> inglenook_error_number;
            
            /// File system error code returned by some boost methods (ex. boost::filesystem::create_directories).
            typedef boost::error_info<struct __boost_filesystem_error, boost::system::error_code> boost_filesystem_error;
            
            /// Error message report by C API errno.
            typedef boost::error_info<struct __c_error_number, int> c_error_number;
            
            /// Error message report by C API strerror(errno).
            typedef boost::error_info<struct __c_error_message, char *> c_error_message;
            
            /// Error message report by win32 API GetLastError()
            typedef boost::error_info<struct __win32_error_number, unsigned long> win32_error_number;
            
            /// Indicates if the code that is in fault is windows code.
            typedef boost::error_info<struct __is_win32_error, bool> is_win32_error;
            
            /**
             * inglenook_exception
             * Standard base exception for all exceptions thrown by objects in inglenook.
             */
            struct inglenook_exception : virtual boost::exception, virtual std::exception
            {
                /// provides a boiler plate explanation of the exception.
                const char* what() const throw() {
                    return boost::locale::translate("Unhandled exception in inglenook").str().c_str();
                }
            };
            
        }
    }
}
