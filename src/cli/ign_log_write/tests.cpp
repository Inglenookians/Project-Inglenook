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
	BOOST_CHECK(parse_action(start_short_action_string) 	== log_write_action::create_new_log);
	BOOST_CHECK(parse_action(start_full_action_string) 		== log_write_action::create_new_log);
	BOOST_CHECK(parse_action(write_short_action_string)		== log_write_action::write_message_to_log);
	BOOST_CHECK(parse_action(write_full_action_string) 		== log_write_action::write_message_to_log);
	BOOST_CHECK(parse_action(close_short_action_string) 	== log_write_action::close_log);
	BOOST_CHECK(parse_action(close_full_action_string) 		== log_write_action::close_log);
	BOOST_CHECK(parse_action(first_bad_action)				== log_write_action::no_action);
	BOOST_CHECK(parse_action(second_bad_action)				== log_write_action::no_action);
		
}

/**
 * Adds an argument to the specified program options object.
 * @params arguments arguments collection to add argument to.
 * @params argument argument to add.
 * @params value value of argument..
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

//
// log_write_tests__parsing_action
// ensures that the library is correctly parsing the action command
BOOST_AUTO_TEST_CASE ( log_write_tests__invoking_actions )
{
	
	boost::program_options::variables_map commandline_arguments;
	add_argument<std::string>(commandline_arguments, "action", "bad");
		
	
}

} // namespace inglenook::logging

} // namespace inglenook


