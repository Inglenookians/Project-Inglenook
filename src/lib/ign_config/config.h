#pragma once

/*
 * config.h: Store and retrieve configuration values from command line, application and global config files.
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
#include <string>

// boost (http://boost.org) includes
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

namespace inglenook
{
    namespace config
    {
        
        /// Storage of command line config file, if specified.
        /// @todo sort how this is being done.
        //extern boost::filesystem::path command_line_config_file;
        
        /**
         * Get a config value. 
         * This returns the first assigned value it finds for the specified key.
         * Config file checked in order: command line, application, global.
         * @param key The key of the config value to retrieve.
         * @param skip_blank Whether to skip over blank values (i.e. "") and keep searching.
         * @param default_value The default value to return if no value set.
         * @return The value for the key.
         */
        boost::optional<std::string> get(const std::string& key, bool skip_blank = true, const boost::optional<std::string>& default_value = boost::optional<std::string>());
        
        // Application config file helpers.
        namespace app
        {
            
            /**
             * Get a config value from the application config file.
             * @param key The key of the config value to retrieve.
             * @param default_value The default value to return if no value set.
             * @return The value for the key.
             */
            boost::optional<std::string> get(const std::string& key, const boost::optional<std::string>& default_value = boost::optional<std::string>());
            
            /**
             * Set a config value to the application config file.
             * @param key The key of the config value to store.
             * @param value The value to set for the key.
             * @return Whether the operation was successfully.
             * @note Ensure that the parent directory of file_path exists and we have permission to write to it.
             */
            bool set(const std::string& key, const std::string& value);
    
            /**
             * Remove a config value from the application config file.
             * @param key The key of the config value to remove.
             * @return Whether the operation was successfully.
             */
            bool remove(const std::string& key);
            
        }
        
        // Global config file helpers.
        namespace global
        {
            
            /**
             * Get a config value from the global config file.
             * @param key The key of the config value to retrieve.
             * @param default_value The default value to return if no value set.
             * @return The value for the key.
             */
            boost::optional<std::string> get(const std::string& key, const boost::optional<std::string>& default_value = boost::optional<std::string>());
            
            /**
             * Set a config value to the global config file.
             * @param key The key of the config value to store.
             * @param value The value to set for the key.
             * @return Whether the operation was successfully.
             * @note Ensure that the parent directory of file_path exists and we have permission to write to it.
             */
            bool set(const std::string& key, const std::string& value);
            
            /**
             * Remove a config value from the gloabl config file.
             * @param key The key of the config value to remove.
             * @return Whether the operation was successfully.
             */
            bool remove(const std::string& key);
            
        }
        
        // Actual config file implementation.
        namespace file
        {
            
            /**
             * Get a config value from a file.
             * @param file_path The location of the configuration file.
             * @param key The key of the config value to retrieve.
             * @param default_value The default value to return if no value set.
             * @return The value for the key.
             */
            boost::optional<std::string> get(const boost::filesystem::path& file_path, const std::string& key, const boost::optional<std::string>& default_value = boost::optional<std::string>());
            
            /**
             * Set a config value to a file.
             * @param file_path The location of the configuration file.
             * @param key The key of the config value to store.
             * @param value The value to set for the key.
             * @return Whether the operation was successfully.
             * @note Ensure that the parent directory of file_path exists and we have permission to write to it.
             */
            bool set(const boost::filesystem::path& file_path, const std::string& key, const std::string& value);
            
            /**
             * Remove a config value from a config file.
             * @param file_path The location of the configuration file.
             * @param key The key of the config value to remove.
             * @return Whether the operation was successfully.
             */
            bool remove(const boost::filesystem::path& file_path, const std::string& key);

        }
    }
}
