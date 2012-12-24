/*
 * tests.cpp: Tests for the ign_config library.
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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ign_core_tests

// standard library includes
#include <fstream>

// boost (http://boost.org) includes
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/locale.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/optional.hpp>

// inglenook includes
#include "config.h"
#include "config_exceptions.h"
#include <ign_core/application.h>
#include <ign_core/environment.h>
#include <ign_core/environment_variables.h>

// Helper functions for manipulating config files during the tests.
namespace
{

    /**
     * Set a config value to a file.
     * @param file_path The location of the configuration file.
     * @param key The key of the config value to store.
     * @param value The value to set for the key.
     * @note Ensure that the parent directory of file_path exists and we have permission to write to it.
     */
    void file_set(const boost::filesystem::path& file_path, const std::string& key, const std::string& value)
    {
        // Generate a new ptree to store the configuration structure.
        boost::property_tree::ptree ptree;
        
        // Quick sanity check to ensure file_path has been set.
        if(file_path.empty())
        {
            // Error! Failed to specify a configuration file.
            std::cerr << boost::locale::translate("ERROR: failed to specify a configuration file") << std::endl;
            
            BOOST_THROW_EXCEPTION(inglenook::core::exceptions::inglenook_exception());
        }
        // Check that the parent folder of the file exists.
        else if(!boost::filesystem::exists(file_path.parent_path()))
        {
            // Error! Specified parent path of configuration file does not exist.
            std::cerr << boost::format(boost::locale::translate("ERROR: specified parent path of configuration file does not exist: '%1%'")) % file_path.parent_path().string() << std::endl;
            
            BOOST_THROW_EXCEPTION(inglenook::core::exceptions::inglenook_exception());
        }
        else
        {
            // Check whether the file exists, we need to touch it if it doesn't for the file lock.
            if(!boost::filesystem::exists(file_path))
            {
                std::ofstream file(file_path.string().c_str());
            }
            
            // Get a lock on the file.
            boost::interprocess::file_lock flock(file_path.string().c_str());
            boost::interprocess::scoped_lock<boost::interprocess::file_lock> exclusive_lock(flock);
            
            // Try and read the current configuration file first into the ptree.
            try
            {
                // Read the configuration file.
                boost::property_tree::xml_parser::read_xml(file_path.string(), ptree);
            }
            catch(boost::property_tree::xml_parser::xml_parser_error &ex)
            {
                // Error! Failed to read the configuration file.
                std::cerr << boost::format(boost::locale::translate("ERROR: failed to read configuration file: '%1%'")) % ex.what() << std::endl;
                
                BOOST_THROW_EXCEPTION(inglenook::core::exceptions::inglenook_exception());
            }
            
            // Try to write the modified configuration structure to file.
            try
            {
                // Set the new value.
                ptree.put(key, value);
                
                // Save the configuration file.
                boost::property_tree::xml_parser::write_xml(file_path.string(), ptree);
            }
            catch(boost::property_tree::ptree_bad_data &ex)
            {
                // Error! Failed to convert value into ptree data type.
                std::cerr << boost::format(boost::locale::translate("ERROR: failed to modify key '%1%' with value '%2%' in the configuration file: '%3%'")) % key % value % ex.what() << std::endl;
                
                BOOST_THROW_EXCEPTION(inglenook::core::exceptions::inglenook_exception());
            }
            catch(boost::property_tree::xml_parser::xml_parser_error &ex)
            {
                // Error! Failed to write the configuration file.
                std::cerr << boost::format(boost::locale::translate("ERROR: failed to write configuration file: '%1%'")) % ex.what() << std::endl;
                
                BOOST_THROW_EXCEPTION(inglenook::core::exceptions::inglenook_exception());
            }
        }
    }

    /**
     * Remove a config value from a file.
     * @param file_path The location of the configuration file.
     * @param key The key of the config value to remove.
     */
    void file_remove(const boost::filesystem::path& file_path, const std::string& key)
    {
        // Generate a new ptree to store the modified configuration structure.
        boost::property_tree::ptree ptree;
        
        // Keep track of whether we have an issue reading in the current congfiguration file.
        auto read_error(false);
        
        // Check whether the file exists.
        if(boost::filesystem::exists(file_path))
        {
            // Get a lock on the file.
            boost::interprocess::file_lock flock(file_path.string().c_str());
            boost::interprocess::scoped_lock<boost::interprocess::file_lock> exclusive_lock(flock);
            
            // Try and read the current configuration file first into the ptree.
            try
            {
                // Read the configuration file.
                boost::property_tree::xml_parser::read_xml(file_path.string(), ptree);
            }
            catch(boost::property_tree::xml_parser::xml_parser_error &ex)
            {
                // Error! Failed to read the configuration file.
                std::cerr << boost::format(boost::locale::translate("ERROR: failed to read configuration file: '%1%'")) % ex.what() << std::endl;
                
                BOOST_THROW_EXCEPTION(inglenook::core::exceptions::inglenook_exception());
            }
            
            // We have to find the parent node of the leaf we want to remove first.
            try
            {
                // Get a pointer to the root.
                boost::property_tree::ptree* find_ptree = &ptree;
                
                // Split the key into its parts.
                std::vector<std::string> key_parts;
                boost::split(key_parts, key, boost::is_any_of("."));
                // Loop through each part, except the last (ie: the leaf we want to delete).
                for(unsigned i = 0; i < key_parts.size() - 1; ++i)
                {
                    // Move the pointer to next part of the key.
                    find_ptree = &(find_ptree->get_child(key_parts[i]));
                }
                
                // We should now be at the leaf's parent, remove the leaf from the parent node.
                find_ptree->erase(key_parts.back());
                
                // Try and write the modified configuration structure to file.
                try
                {
                    // Save the configuration file.
                    boost::property_tree::xml_parser::write_xml(file_path.string(), ptree);
                }
                catch(boost::property_tree::ptree_bad_data &ex)
                {
                    // Error! Failed to convert value into ptree data type.
                    std::cerr << boost::format(boost::locale::translate("ERROR: failed to remove key '%1%' in the configuration file: '%2%'")) % key % ex.what() << std::endl;
                    
                    BOOST_THROW_EXCEPTION(inglenook::core::exceptions::inglenook_exception());
                }
                catch(boost::property_tree::xml_parser::xml_parser_error &ex)
                {
                    // Error! Failed to write the configuration file.
                    std::cerr << boost::format(boost::locale::translate("ERROR: failed to write configuration file: '%1%'")) % ex.what() << std::endl;
                    
                    BOOST_THROW_EXCEPTION(inglenook::core::exceptions::inglenook_exception());
                }
            }
            catch(boost::property_tree::ptree_bad_path &ex)
            {
                // Warning! Trying to remove a key that does not exist.
                std::cerr << boost::format(boost::locale::translate("WARNING: trying to remove key that does not exist: '%1%'")) % file_path.string() << std::endl;
                
                // Technically, this is still a success so no need to throw an exception!
            }
        }
        else
        {
            // Error! Configuration files does not exist.
            std::cerr << boost::format(boost::locale::translate("ERROR: configuration file does not exist: '%1%'")) % file_path.string() << std::endl;
            
            BOOST_THROW_EXCEPTION(inglenook::core::exceptions::inglenook_exception());
        }
    }
}

// config test case.
BOOST_AUTO_TEST_CASE(config)
{
    // Get the location of the system temp directory.
    auto temp_directory(boost::filesystem::temp_directory_path());
    // Test directly loading a cache.
    auto test_file_path(temp_directory / "cache_file.xml");
    // Load an empty file first. Size of ptree should be zero, an empty optional string should be returned.
    boost::property_tree::ptree load_test = inglenook::config::cache::load(test_file_path);
    BOOST_CHECK(load_test.size() == 0);
    BOOST_CHECK(inglenook::config::cache::get(load_test, "key") == boost::optional<std::string>());
    // Load a file with a config setting. Size of ptree should be one, a correctly populated string should be returned.
    BOOST_CHECK_NO_THROW(file_set(test_file_path, "key", "value"));
    load_test = inglenook::config::cache::load(test_file_path);
    BOOST_CHECK(load_test.size() == 1);
    BOOST_CHECK(inglenook::config::cache::get(load_test, "key") == boost::optional<std::string>("value"));
    // Cleanup config file.
    boost::filesystem::remove(test_file_path);

    // Get the application, global and command line config file names.
    auto application_file_path(temp_directory / (inglenook::core::application::name() + "_config.xml"));
    auto global_file_path(temp_directory / "config.xml");
    auto command_line_file_path(temp_directory / "command_line.xml");

    // Override the config location with the temp directory (affects global and app functions).
    inglenook::core::environment::set(inglenook::core::environment::variables::DIR_CONFIG, temp_directory.string());

    // Test the config file locations.
    BOOST_CHECK(inglenook::config::app::filepath() == application_file_path);
    BOOST_CHECK(inglenook::config::global::filepath() == global_file_path);

    // Test direct get from the command line file config.
    // First setup a config with command line file option set.
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key", "command_file"));
    inglenook::config::cache::clear();
    // Set the command line config, so that it is used.
    inglenook::core::application::config_file(command_line_file_path);
    // Test getting from the command line config.
    BOOST_CHECK(inglenook::config::command_line::get("key") == boost::optional<std::string>("command_file"));
    // Cleanup config file.
    inglenook::core::application::config_file("");
    boost::filesystem::remove(command_line_file_path);
    inglenook::config::cache::clear();

    // Test direct get from the application config.
    // First setup a config with application file option set.
    BOOST_CHECK_NO_THROW(file_set(application_file_path, "key", "application"));
    inglenook::config::cache::clear();
    // Test getting from the app config.
    BOOST_CHECK(inglenook::config::app::get("key") == boost::optional<std::string>("application"));
    // Cleanup config file.
    boost::filesystem::remove(application_file_path);
    inglenook::config::cache::clear();

    // Test direct get from the global config.
    // First setup a config with global file option set.
    BOOST_CHECK_NO_THROW(file_set(global_file_path, "key", "global"));
    inglenook::config::cache::clear();
    // Test getting from the global config.
    BOOST_CHECK(inglenook::config::global::get("key") == boost::optional<std::string>("global"));
    // Cleanup config file.
    boost::filesystem::remove(global_file_path);
    inglenook::config::cache::clear();

    // Test different combined get situations.
    // No global, app or command line file configs.
    // Test getting with defaults, should return an empty boost::optional.
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key", true) == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key", false) == boost::optional<std::string>());
    // Test getting with specified default value, return specified default value.
    BOOST_CHECK(inglenook::config::get("key", true, std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::get("key", false, std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("value")) == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key", false, std::string("value")) == boost::optional<std::string>("value"));
    // Test getting with defaults, should return an empty boost::optional.
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>());
    // Test getting with specified default value, return specified default value.
    BOOST_CHECK(inglenook::config::get("key", true, std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("default")) == boost::optional<std::string>("default"));
    // Test setting a value.
    // Set the command line config, so that it is used.
    inglenook::core::application::config_file(command_line_file_path);
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key", "value"));
    inglenook::config::cache::clear();
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("")) == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("default")) == boost::optional<std::string>("value"));
    // Test removing a key.
    BOOST_CHECK_NO_THROW(file_remove(command_line_file_path, "key"));
    inglenook::config::cache::clear();
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key", true, std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("default")) == boost::optional<std::string>("default"));
    // Test multi-level key set, remove then get.
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key", "value"));
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key.a", "value"));
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key.a.1", "value"));
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key.a.2", "value"));
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key.b", "value"));
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key.b.1", "value"));
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key.b.2", "value"));
    // Test removing multi-level child.
    BOOST_CHECK_NO_THROW(file_remove(command_line_file_path, "key.a"));
    inglenook::config::cache::clear();
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key.a") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.a.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.a.2") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.b") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key.b.1") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key.b.2") == boost::optional<std::string>("value"));
    // Test removing leaf of multi-level child.
    BOOST_CHECK_NO_THROW(file_remove(command_line_file_path, "key.b.1"));
    inglenook::config::cache::clear();
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key.a") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.a.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.a.2") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.b") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key.b.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.b.2") == boost::optional<std::string>("value"));
    // Test removing root of multi-level child.
    BOOST_CHECK_NO_THROW(file_remove(command_line_file_path, "key"));
    inglenook::config::cache::clear();
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.a") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.a.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.a.2") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.b") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.b.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key.b.2") == boost::optional<std::string>());
    // Cleanup direct config file interface.
    inglenook::core::application::config_file("");
    boost::filesystem::remove(command_line_file_path);
    inglenook::config::cache::clear();

    // Test global, application, command line file and command line arguments configs.
    // Set global and application settings first.
    BOOST_CHECK_NO_THROW(file_set(global_file_path, "key", "global"));
    BOOST_CHECK_NO_THROW(file_set(application_file_path, "key", "application"));
    inglenook::config::cache::clear();
    // Set the command line file config.
    inglenook::core::application::config_file(command_line_file_path);
    // Test getting, command line file config should not override value.
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("application"));
    // Set the command line file config, this should override value.
    BOOST_CHECK_NO_THROW(file_set(command_line_file_path, "key", "command_line_file"));
    inglenook::config::cache::clear();
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("command_line_file"));
    // Set the command line arguments config, this should override value.
    std::map<std::string, std::string> cl_arguments;
    cl_arguments["key"] = "command_line_arguments";
    inglenook::core::application::config_arguments(cl_arguments);
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("command_line_arguments"));
    // Disable the command line arguments and file configs, all our previous global and application values should reappear.
    inglenook::core::application::config_arguments(std::map<std::string, std::string>());
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("command_line_file"));
    inglenook::core::application::config_file("");
    inglenook::config::cache::clear();
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("application"));
    BOOST_CHECK_NO_THROW(file_remove(application_file_path, "key"));
    inglenook::config::cache::clear();
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("global"));
    // Cleanup global and app config file interface.
    boost::filesystem::remove(global_file_path);
    boost::filesystem::remove(application_file_path);
    boost::filesystem::remove(command_line_file_path);
    inglenook::config::cache::clear();
}
