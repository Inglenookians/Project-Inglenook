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
#include <boost/property_tree/ptree.hpp>

namespace inglenook
{
    namespace config
    {
        
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
        
        // Command line config file helpers.
        namespace command_line
        {
            
            /**
             * Get a config value from the command line config file.
             * @param key The key of the config value to retrieve.
             * @param default_value The default value to return if no value set.
             * @return The value for the key.
             */
            boost::optional<std::string> get(const std::string& key, const boost::optional<std::string>& default_value = boost::optional<std::string>());
            
        }
        
        // Application config file helpers.
        namespace app
        {
            
            /**
             * The application config filepath.
             * @return Filepath of the application config file.
             */
            boost::filesystem::path filepath();
            
            /**
             * Get a config value from the application config file.
             * @param key The key of the config value to retrieve.
             * @param default_value The default value to return if no value set.
             * @return The value for the key.
             */
            boost::optional<std::string> get(const std::string& key, const boost::optional<std::string>& default_value = boost::optional<std::string>());
            
        }
        
        // Global config file helpers.
        namespace global
        {

            /**
             * The global config filepath.
             * @return Filepath of the global config file.
             */
            boost::filesystem::path filepath();

            /**
             * Get a config value from the global config file.
             * @param key The key of the config value to retrieve.
             * @param default_value The default value to return if no value set.
             * @return The value for the key.
             */
            boost::optional<std::string> get(const std::string& key, const boost::optional<std::string>& default_value = boost::optional<std::string>());
            
        }

        // Caching config file implementation.
        namespace cache
        {

            /**
             * Load the full configuration settings from a file.
             * @param file_path The location of the configuration file.
             * @return The configuration settings.
             */
            boost::property_tree::ptree load(const boost::filesystem::path& file_path);
            
            /**
             * Get a config value from a cache.
             * @param ptree The cache to use.
             * @param key The key of the config value to retrieve.
             * @param default_value The default value to return if no value set.
             * @return The value for the key.
             */
            boost::optional<std::string> get(const boost::property_tree::ptree& ptree, const std::string& key, const boost::optional<std::string>& default_value = boost::optional<std::string>());
            
            /**
             * Clear the configuration setting caches.
             */
            void clear();
            
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
             * @note Ensure that the parent directory of file_path exists and we have permission to write to it.
             */
            void set(const boost::filesystem::path& file_path, const std::string& key, const std::string& value);
            
            /**
             * Remove a config value from a config file.
             * @param file_path The location of the configuration file.
             * @param key The key of the config value to remove.
             */
            void remove(const boost::filesystem::path& file_path, const std::string& key);

        }
    }
}
