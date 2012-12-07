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
            
            /// Added to exceptions when a config file's parent directory does not exist.
            typedef boost::error_info<struct __config_file_parent, boost::filesystem::path> config_file_parent;
            
            /// Added to exceptions when a config file's value cannot be converted.
            typedef boost::error_info<struct __config_file_key, std::string> config_file_key;
            
            /// Added to exceptions when a config file's value cannot be converted.
            typedef boost::error_info<struct __config_file_value, std::string> config_file_value;
            
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
            
            /**
             * config_file_not_found_exception
             * This exception is thrown when the specified config file does not exist.
             */
            struct config_file_not_found_exception : virtual inglenook::core::exceptions::inglenook_exception
            {
                /// provides a boiler plate explanation of the exception.
                const char* what() const throw() {
                    return boost::locale::translate("Failed to open specified config file").str().c_str();
                }
            };
            
            /**
             * config_file_parent_exception
             * This exception is thrown when the specified config file's parent directory does not exist.
             */
            struct config_file_parent_exception : virtual inglenook::core::exceptions::inglenook_exception
            {
                /// provides a boiler plate explanation of the exception.
                const char* what() const throw() {
                    return boost::locale::translate("Parent directory of config file does not exist").str().c_str();
                }
            };
            
            /**
             * config_file_value_exception
             * This exception is thrown when the specified value cannot be converted for saving.
             */
            struct config_file_value_exception : virtual inglenook::core::exceptions::inglenook_exception
            {
                /// provides a boiler plate explanation of the exception.
                const char* what() const throw() {
                    return boost::locale::translate("There was a problem converting the value for saving").str().c_str();
                }
            };
            
            /**
             * config_file_write_exception
             * This exception is thrown when issues are encountered with writing to the config file.
             */
            struct config_file_write_exception : virtual inglenook::core::exceptions::inglenook_exception
            {
                /// provides a boiler plate explanation of the exception.
                const char* what() const throw() {
                    return boost::locale::translate("There was a problem writing to the config file").str().c_str();
                }
            };
            
        }
    }
}
