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

// boost includes
#include <boost/test/unit_test.hpp>

// inglenook includes
#include "config.h"
#include <ign_core/application.h>
#include <ign_core/environment.h>
#include <ign_core/environment_variables.h>

// config test case.
BOOST_AUTO_TEST_CASE(config)
{
    // Get the location of the system temp directory.
    auto temp_directory = boost::filesystem::temp_directory_path();
    // Get the global, application and command line config file names.
    auto global_file_path = temp_directory / "config.xml";
    auto application_file_path = temp_directory / (inglenook::core::application::name() + "_config.xml");
    auto command_line_file_path = temp_directory / "command_line.xml";
    
    // Test the direct file interface.
    auto temp_file_path = temp_directory / "direct_file.xml";
    // Test getting with defaults, should return an empty boost::optional.
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key") == boost::optional<std::string>());
    // Test getting with specified default value, return specified default value.
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key", std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key", std::string("default")) == boost::optional<std::string>("default"));
    // Test setting a value.
    BOOST_CHECK(inglenook::config::file::set(temp_file_path, "key", "value"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key", std::string("")) == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key", std::string("default")) == boost::optional<std::string>("value"));
    // Test removing a key.
    BOOST_CHECK(inglenook::config::file::remove(temp_file_path, "key"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key", std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key", std::string("default")) == boost::optional<std::string>("default"));
    // Test multi-level key set, remove then get.
    BOOST_CHECK(inglenook::config::file::set(temp_file_path, "key", "value"));
    BOOST_CHECK(inglenook::config::file::set(temp_file_path, "key.a", "value"));
    BOOST_CHECK(inglenook::config::file::set(temp_file_path, "key.a.1", "value"));
    BOOST_CHECK(inglenook::config::file::set(temp_file_path, "key.a.2", "value"));
    BOOST_CHECK(inglenook::config::file::set(temp_file_path, "key.b", "value"));
    BOOST_CHECK(inglenook::config::file::set(temp_file_path, "key.b.1", "value"));
    BOOST_CHECK(inglenook::config::file::set(temp_file_path, "key.b.2", "value"));
    // Test removing multi-level child.
    BOOST_CHECK(inglenook::config::file::remove(temp_file_path, "key.a"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.a") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.a.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.a.2") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.b") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.b.1") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.b.2") == boost::optional<std::string>("value"));
    // Test removing leaf of multi-level child.
    BOOST_CHECK(inglenook::config::file::remove(temp_file_path, "key.b.1"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.a") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.a.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.a.2") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.b") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.b.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.b.2") == boost::optional<std::string>("value"));
    // Test removing root of multi-level child.
    BOOST_CHECK(inglenook::config::file::remove(temp_file_path, "key"));
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.a") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.a.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.a.2") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.b") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.b.1") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::file::get(temp_file_path, "key.b.2") == boost::optional<std::string>());
    // Cleanup direct config file interface.
    boost::filesystem::remove(temp_file_path);
    
    // Override the config location with the temp directory (affects global and app functions).
    inglenook::core::environment::set(inglenook::core::environment::variables::DIR_CONFIG, temp_directory.string());
    
    // No global, app or command line config file.
    // Test getting with defaults, should return an empty boost::optional.
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key", true) == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key", false) == boost::optional<std::string>());
    // Test getting with specified default value, return specified default value.
    BOOST_CHECK(inglenook::config::get("key", true, std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::get("key", false, std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("value")) == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key", false, std::string("value")) == boost::optional<std::string>("value"));
    
    // Test global only config file.
    // Test setting a global variable.
    BOOST_CHECK(inglenook::config::global::set("key", "value"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("")) == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("default")) == boost::optional<std::string>("value"));
    // Test removing a key.
    BOOST_CHECK(inglenook::config::global::remove("key"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key", true, std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("default")) == boost::optional<std::string>("default"));
    // Cleanup global config file interface.
    boost::filesystem::remove(global_file_path);
    
    // Test application only config file.
    // Test setting an application variable.
    BOOST_CHECK(inglenook::config::app::set("key", "value"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("")) == boost::optional<std::string>("value"));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("default")) == boost::optional<std::string>("value"));
    // Test removing a key.
    BOOST_CHECK(inglenook::config::app::remove("key"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::get("key", true, std::string("")) == boost::optional<std::string>(""));
    BOOST_CHECK(inglenook::config::get("key", true, std::string("default")) == boost::optional<std::string>("default"));
    // Cleanup app config file interface.
    boost::filesystem::remove(application_file_path);
    
    // Test global and appplication config file.
    // Test setting both global and appplication variables, application should hide global values.
    BOOST_CHECK(inglenook::config::global::set("key", "global"));
    BOOST_CHECK(inglenook::config::app::set("key", "application"));
    BOOST_CHECK(inglenook::config::global::set("key.global", "global"));
    BOOST_CHECK(inglenook::config::app::set("key.application", "application"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("application"));
    BOOST_CHECK(inglenook::config::get("key.global") == boost::optional<std::string>("global"));
    BOOST_CHECK(inglenook::config::get("key.application") == boost::optional<std::string>("application"));
    // Remove the application keys, should expose appropriate global keys.
    BOOST_CHECK(inglenook::config::app::remove("key.application"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("application"));
    BOOST_CHECK(inglenook::config::get("key.global") == boost::optional<std::string>("global"));
    BOOST_CHECK(inglenook::config::get("key.application") == boost::optional<std::string>());
    BOOST_CHECK(inglenook::config::app::remove("key"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("global"));
    BOOST_CHECK(inglenook::config::get("key.global") == boost::optional<std::string>("global"));
    // Cleanup global and app config file interface.
    boost::filesystem::remove(global_file_path);
    boost::filesystem::remove(application_file_path);
    
    // Test command line config file.
    // Set global and application settings.
    BOOST_CHECK(inglenook::config::global::set("key", "global"));
    BOOST_CHECK(inglenook::config::app::set("key", "application"));
    // Set the command line config.
    inglenook::core::application::config_file(command_line_file_path);
    // Test getting, command line config file should override with no values.
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>());
    // Test setting, command line config file should override be altered.
    // Test setting an application setting.
    BOOST_CHECK(inglenook::config::app::set("key", "command_line_application"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("command_line_application"));
    // Test setting an application setting. As command line has taken over all, this will actually change the value.
    BOOST_CHECK(inglenook::config::global::set("key", "command_line_global"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("command_line_global"));
    // Disable the command line config file, all our previous global and application values should reappear.
    inglenook::core::application::config_file("");
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("application"));
    BOOST_CHECK(inglenook::config::app::remove("key"));
    BOOST_CHECK(inglenook::config::get("key") == boost::optional<std::string>("global"));
    // Cleanup global and app config file interface.
    boost::filesystem::remove(global_file_path);
    boost::filesystem::remove(application_file_path);
    boost::filesystem::remove(command_line_file_path);
}
