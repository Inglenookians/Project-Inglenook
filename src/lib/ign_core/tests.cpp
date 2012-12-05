/*
 * tests.cpp: Tests for the ign_core library.
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
#include <iostream>

// boost includes
#include <boost/test/unit_test.hpp>

// inglenook includes
#include "application.h"
#include "application_exceptions.h"
#include "environment.h"

// application test case.
BOOST_AUTO_TEST_CASE(application)
{
    // Test the application description, version and build before creation.
    BOOST_CHECK(inglenook::core::application::description() == "");
    BOOST_CHECK(inglenook::core::application::version() == "");
    BOOST_CHECK(inglenook::core::application::build() == "");
    
    // Create the application.
    inglenook::core::application app("description", "version", "build_time", "build_date");
    
    // Test the application description, version and build after creation.
    BOOST_CHECK(inglenook::core::application::description() == "description");
    BOOST_CHECK(inglenook::core::application::version() == "version");
    BOOST_CHECK(inglenook::core::application::build() == "build_time build_date");
    
    // Test the configuration file.
    BOOST_CHECK(inglenook::core::application::config_file() == "");
    inglenook::core::application::config_file("config_file");
    BOOST_CHECK(inglenook::core::application::config_file() == "config_file");
    inglenook::core::application::config_file("");
    BOOST_CHECK(inglenook::core::application::config_file() == "");
    
    // Test the application name (we know this!)
    BOOST_CHECK(inglenook::core::application::name() == "ign_core_tests");
    
    // It's difficult to test whether the pid is correct, just check a valid pid is returned (ie: pid > 0)
    BOOST_CHECK(inglenook::core::application::pid() > 0);
    
    /// @todo test for arguments parser.
    
    // Test the arguments parser.
    // Setup some required variables.
    boost::program_options::variables_map variables_map_one;
    const char* argv_one[] { "--help", "--help", "--custom" };
    boost::program_options::variables_map variables_map_two;
    const char* argv_two[] { "appname", "--version", "custom" };
    boost::program_options::variables_map variables_map_three;
    const char* argv_three[] { "appname", "--config-file", "conf_file.xml" };
    boost::program_options::options_description options;
    options.add_options() ("custom", "Custom Option");
    boost::program_options::positional_options_description positions;
    positions.add("custom", 1);
    
    // Test no arguments, all options should be ignored.
    BOOST_CHECK(inglenook::core::application::arguments_parser(variables_map_one, 0, argv_one) == false);
    // Test one argument, all options should still be ignored even though help option specified.
    BOOST_CHECK(inglenook::core::application::arguments_parser(variables_map_one, 1, argv_one) == false);
    // Test two arguments, with the --help option specified.
    BOOST_CHECK(inglenook::core::application::arguments_parser(variables_map_one, 2, argv_one) == true);
    
    // Test two arguments, with the --version option specified.
    BOOST_CHECK(inglenook::core::application::arguments_parser(variables_map_two, 2, argv_two) == true);
    
    // Test two arguments, with the --config-file specified but with no value, should throw.
    BOOST_CHECK_THROW(inglenook::core::application::arguments_parser(variables_map_three, 2, argv_three), inglenook::core::exceptions::application_arguments_parser_exception);
    // Test three arguments, with the --config-file specifed with a value.
    BOOST_CHECK(inglenook::core::application::arguments_parser(variables_map_three, 3, argv_three) == false);
    
    // Test three arguments, with the --custom option specified but not added as an option, should throw.
    BOOST_CHECK_THROW(inglenook::core::application::arguments_parser(variables_map_one, 3, argv_one), inglenook::core::exceptions::application_arguments_parser_exception);
    // Test three arguments, with the --custom option specified and it has been added as an option.
    BOOST_CHECK_NO_THROW(inglenook::core::application::arguments_parser(variables_map_one, 3, argv_one, options));
    
    // Test three arguments, with the positional custom option specified but not added as a option/positional option, should throw.
    BOOST_CHECK_THROW(inglenook::core::application::arguments_parser(variables_map_two, 3, argv_two), inglenook::core::exceptions::application_arguments_parser_exception);
    // Test three arguments, with the positional custom option specified and it has been added as an option but not positional, show throw.
    BOOST_CHECK_THROW(inglenook::core::application::arguments_parser(variables_map_two, 3, argv_two, options), inglenook::core::exceptions::application_arguments_parser_exception);
    // Test three arguments, with the positional custom option specifeid and it has been added as an option/positional.
    BOOST_CHECK_NO_THROW(inglenook::core::application::arguments_parser(variables_map_two, 3, argv_two, options, positions));
}

// environment test case.
BOOST_AUTO_TEST_CASE(environment)
{
    // Test getting and setting environment variable.
    std::string variable_one = "INGLENOOK_TEST_VARIABLE_ONE";
    std::string variable_two = "INGLENOOK_TEST_VARIABLE_TWO";
    
    // Test fetching with no default parameter.
    BOOST_CHECK(inglenook::core::environment::get(variable_one) == "");
    
    // Test fetching with no default parameter.
    BOOST_CHECK(inglenook::core::environment::get(variable_one, "DEFAULT") != "");
    BOOST_CHECK(inglenook::core::environment::get(variable_one, "DEFAULT") == "DEFAULT");
    
    // Test setting with overwrite enabled.
    BOOST_CHECK_NO_THROW(inglenook::core::environment::set(variable_one, "ONE", true));
    BOOST_CHECK(inglenook::core::environment::get(variable_one) == "ONE");
    BOOST_CHECK_NO_THROW(inglenook::core::environment::set(variable_one, "TWO", true));
    BOOST_CHECK(inglenook::core::environment::get(variable_one) != "ONE");
    BOOST_CHECK(inglenook::core::environment::get(variable_one) == "TWO");
    
    // Test setting with overwrite disabled for pre-existing variable.
    BOOST_CHECK_NO_THROW(inglenook::core::environment::set(variable_one, "THREE", false));
    BOOST_CHECK(inglenook::core::environment::get(variable_one) != "ONE");
    BOOST_CHECK(inglenook::core::environment::get(variable_one) == "TWO");
    BOOST_CHECK(inglenook::core::environment::get(variable_one) != "THREE");
    
    // Test setting with overwrite disabled for new variable.
    BOOST_CHECK_NO_THROW(inglenook::core::environment::set(variable_two, "APPLE", false));
    BOOST_CHECK(inglenook::core::environment::get(variable_two) == "APPLE");
    BOOST_CHECK_NO_THROW(inglenook::core::environment::set(variable_two, "ORANGE", false));
    BOOST_CHECK(inglenook::core::environment::get(variable_two) == "APPLE");
    BOOST_CHECK(inglenook::core::environment::get(variable_two) != "ORANGE");
}
