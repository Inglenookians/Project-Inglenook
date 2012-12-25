#pragma once

/*
 * environment_exceptions.h: Defines environment exceptions.
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
            /// environment variable name that is being set at time of exception.
            typedef boost::error_info<struct __environment_variable_name, std::string> environment_variable_name;
            
            /// environment varibale value that is being set at time of exception.
            typedef boost::error_info<struct __environment_variable_value, std::string> environment_variable_value;

            /**
             * set_environment_exception
             * This exception is thrown when issues are encountered with setting an environment variable.
             */
            struct set_environment_exception : virtual inglenook::core::exceptions::inglenook_exception
            {
                /// provides a boiler plate explanation of the exception.
                const char* what() const throw() {
                    return boost::locale::translate("There was a problem setting an environment variable").str().c_str();
                }
            };

        }
    }
}
