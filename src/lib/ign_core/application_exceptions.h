#pragma once

/*
 * application_exceptions.h: Defines application exceptions.
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
#include "exceptions.h"

namespace inglenook
{
    namespace core
    {  
        namespace exceptions
        {

            /// Added to exceptions when a process cmdline file handle couldn't be opened..
            typedef boost::error_info<struct __application_cmdline_file, boost::filesystem::path> application_cmdline_file;
            
            /**
             * application_name_exception
             * This exception is thrown when issues are encountered with a process name, this might occur
             * if the process name cannot be determined, or is not valid.
             */
            struct application_name_exception : virtual inglenook::core::exceptions::inglenook_exception
            {
                /// provides a boiler plate explanation of the exception.
                const char* what() const throw() {
                    return boost::locale::translate("There was a problem determining the processes real name").str().c_str();
                }
            };

            /**
             * application_arguments_parser_exception
             * This exception is thrown when the command line parser is given invalid arguments.
             */
            struct application_arguments_parser_exception : virtual inglenook::core::exceptions::inglenook_exception
            {
                /// provides a boiler plate explanation of the exception.
                const char* what() const throw() {
                    return boost::locale::translate("There was a problem processing the command line arguments").str().c_str();
                }
            };

        }
    }
}
