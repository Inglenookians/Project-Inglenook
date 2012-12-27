/*
* tests.cpp: Test routines for the ign_log_write assembly (src/cli/ign_log_write)
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
#define BOOST_TEST_MODULE ign_log_write_tests

/*
 * The following tests help ensure that ign_log_write is operating correctly.
 * Its covers behaviours specific to the application. The follow elements are
 * not currently tested:
 *  log_write_exceptions.h  (all)     - no functional code.
 *  log_write_actions.h/cpp (listed)  - methods touch disk, and are largely tested by ign_logging already.
 *      create_log_file()               | all prototypes.
 *      write_log_entry()               | all prototypes.
 *      close_log_file()                | all prototypes.
 *  main.cpp                          - little functional code tests could be written for (by design).
 */


// boost (http://boost.org) includes
#include <boost/test/unit_test.hpp>

// inglenook includes
#include "log_write_actions.h"

namespace inglenook
{

namespace logging
{

//
// log_write_tests__argument_strings
// where changing the values of the command arguments its best to be aware these have knock on 
// effects for documentation and scripts that use them. this test is designed to make sure the
// values are no "idly" changed without noting the impacts it might have.
BOOST_AUTO_TEST_CASE ( log_write_tests__argument_strings )
{
    // check default values for start log
    BOOST_CHECK_MESSAGE(start_full_action_string == "start",
            "start log (full) action string  has been changed from 'start' to '" <<
            start_full_action_string << "', please make sure all documentation is updated");
    BOOST_CHECK_MESSAGE(start_short_action_string == "s",
            "start log (short) action string  has been changed from 's' to '" <<
            start_short_action_string << "', please make sure all documentation is updated");

    // check default values for write log
    BOOST_CHECK_MESSAGE(write_full_action_string == "write",
            "write log (full) action string  has been changed from 'write' to '" <<
            write_full_action_string << "', please make sure all documentation is updated");
    BOOST_CHECK_MESSAGE(write_short_action_string == "w",
            "write log (short) action string  has been changed from 'w' to '" <<
            write_short_action_string << "', please make sure all documentation is updated");
    
    // check default values for close log
    BOOST_CHECK_MESSAGE(close_full_action_string == "close",
            "close log (full) action string  has been changed from 'close' to '" <<
            start_full_action_string << "', please make sure all documentation is updated");
    BOOST_CHECK_MESSAGE(close_short_action_string == "c",
            "close log (short) action string has been changed from 'c' to '" <<
            close_short_action_string << "', please make sure all documentation is updated");
}


//
// log_write_tests__parsing_action
// ensures that the library is correctly parsing the action command
BOOST_AUTO_TEST_CASE ( log_write_tests__parsing_action )
{
    const std::string first_bad_action = "wrong";
    const std::string second_bad_action = "";

    // check default values
    BOOST_CHECK(parse_action(start_short_action_string)  == log_write_action::create_new_log);
    BOOST_CHECK(parse_action(start_full_action_string)   == log_write_action::create_new_log);
    BOOST_CHECK(parse_action(write_short_action_string)  == log_write_action::write_message_to_log);
    BOOST_CHECK(parse_action(write_full_action_string)   == log_write_action::write_message_to_log);
    BOOST_CHECK(parse_action(close_short_action_string)  == log_write_action::close_log);
    BOOST_CHECK(parse_action(close_full_action_string)   == log_write_action::close_log);
    BOOST_CHECK(parse_action(first_bad_action)           == log_write_action::no_action);
    BOOST_CHECK(parse_action(second_bad_action)          == log_write_action::no_action);
}

/**
 * Adds an argument to the specified program options object.
 * @param arguments arguments collection to add argument to.
 * @param argument argument to add.
 * @param value value of argument..
 */
template <class T> void add_argument(boost::program_options::variables_map& arguments, std::string argument, T value)
{
    // add the argument to the collection
    arguments.insert(  
        std::pair<std::string, boost::program_options::variable_value>(
            argument, boost::program_options::variable_value( boost::any(value), false )
        )
    );
}

/**
 * Submits a require instruction which is expected to fail.
 * @param arguments arguments collection to query.
 * @param argument argument to find (or not as case may be).
 */
template <class T> void require_and_expect_missing(boost::program_options::variables_map& arguments, std::string argument)
{
    try
    {
        // attempt the test - for science.
        auto result = require_parameter<T>(arguments, argument);
        BOOST_FAIL("Expected require_paramater() on arguments to fail. Actually yielded \"" << result << "\"");
    }
    catch(inglenook::logging::action_required_arguments_missing_exception)
    {
        // nothing to do - this is the correct result //
    }
}

//
// log_write_tests__require_parameters
// ensures that the library is correctly extracting required parameters
BOOST_AUTO_TEST_CASE ( log_write_tests__require_parameters )
{
    // build a variable map to test with
    boost::program_options::variables_map commandline_arguments;
    add_argument<std::string>  (commandline_arguments, "action1", "key1");
    add_argument<std::string>  (commandline_arguments, "action2", "key2");
    add_argument<unsigned int> (commandline_arguments, "action3", 3);

    // check failure is working as intended.
    require_and_expect_missing<std::string>(commandline_arguments, "actions4");
    require_and_expect_missing<std::string>(commandline_arguments, "actions5");
    require_and_expect_missing<unsigned int>(commandline_arguments, "actions6");

    // check success if working as intended.
    BOOST_CHECK(require_parameter<std::string>(commandline_arguments, "action1") == "key1" );
    BOOST_CHECK(require_parameter<std::string>(commandline_arguments, "action2") == "key2" );
    BOOST_CHECK(require_parameter<unsigned int>(commandline_arguments, "action3") == 3 );
}

//
// log_write_tests__optional_parameters
// ensures that the library is correctly extracting optional parameters
BOOST_AUTO_TEST_CASE ( log_write_tests__optional_parameters )
{
    // build a variable map to test with
    boost::program_options::variables_map commandline_arguments;
    add_argument<std::string>  (commandline_arguments, "action1", "key1");
    add_argument<std::string>  (commandline_arguments, "action2", "key2");
    add_argument<unsigned int> (commandline_arguments, "action3", 3);

    // check failure is working as intended.
    BOOST_CHECK(optional_parameter<std::string>(commandline_arguments, "action4", "fallback") == "fallback" );
    BOOST_CHECK(optional_parameter<std::string>(commandline_arguments, "action5", "fallback") == "fallback" );
    BOOST_CHECK(optional_parameter<unsigned int>(commandline_arguments, "action6", 999) == 999 );

    // check success if working as intended.
    BOOST_CHECK(optional_parameter<std::string>(commandline_arguments, "action1", "fallback") == "key1" );
    BOOST_CHECK(optional_parameter<std::string>(commandline_arguments, "action2", "fallback") == "key2" );
    BOOST_CHECK(optional_parameter<unsigned int>(commandline_arguments, "action3", 999) == 3 );
}

//
// log_write_tests__convert_category_tests
// ensures that the convert to category tests are working
BOOST_AUTO_TEST_CASE ( log_write_tests__convert_category_tests )
{
    // first check all known good values
    BOOST_CHECK( convert_to_category(1) == category::debugging);
    BOOST_CHECK( convert_to_category(2) == category::verbose);
    BOOST_CHECK( convert_to_category(3) == category::information);
    BOOST_CHECK( convert_to_category(4) == category::warning);
    BOOST_CHECK( convert_to_category(5) == category::error);
    BOOST_CHECK( convert_to_category(6) == category::fatal);

    // next check known values that should not convert
    BOOST_CHECK( convert_to_category(0)    == category::information); // unspecified
    BOOST_CHECK( convert_to_category(0x99) == category::information); // no_log

    // finally just throw a whole range at it
    for(int i = 7; i < 0xff; i++)
    {
        // check that the invalid values don't cast
        BOOST_CHECK( convert_to_category(i) == category::information);
    }
}

} // namespace inglenook::logging

} // namespace inglenook


