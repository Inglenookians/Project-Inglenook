#pragma once

/*
 * config_exceptions.h: Defines config exceptions.
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
#include <ign_core/exceptions.h>

namespace inglenook
{
    namespace config
    {  
        namespace exceptions
        {
            
            /// Added to exceptions when a config file cannot be read/written.
            typedef boost::error_info<struct __config_file, boost::filesystem::path> config_file;
            
            /**
             * config_file_read_exception
             * This exception is thrown when issues are encountered with reading the config file.
             */
            struct config_file_read_exception : virtual inglenook::core::exceptions::inglenook_exception
            {
                /// provides a boiler plate explanation of the exception.
                const char* what() const throw() {
                    return boost::locale::translate("There was a problem reading the config file").str().c_str();
                }
            };
            
        }
    }
}
