#pragma once
/*
* log_entry_tests.h: Test routines for the log_entry class (log_entry.cpp/h)
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
#define BOOST_TEST_MODULE LOG_TEST_NAME

// boost (http://boost.org) includes
#include <boost/test/unit_test.hpp>

// inglenook includes
#include "log_entry.h"

namespace inglenook
{

namespace logging
{

//
// log_entry_tests__defaults
// checks all the default values for correctness. This should ensure that
// the default values are predictable, and that the read accessors appear
// to be working as expected (these will be further tested).
BOOST_AUTO_TEST_CASE ( log_entry_tests__defaults )
{
	log_entry _log_entry;

	// check default values
	BOOST_CHECK(_log_entry.entry_type() == category::unspecified);
	BOOST_CHECK(_log_entry.log_namespace() == "");
	BOOST_CHECK(_log_entry.message() == "");
	BOOST_CHECK(_log_entry.extended_data().size() == 0);

}

//
// log_entry_tests__property_assignments
// checks that assignment and recall of member variables is working as
// expected. This is usually done several times over to ensure multiple assignments
// do not cause a fault, or values of certain lengths / conditions behave as expected.
BOOST_AUTO_TEST_CASE ( log_entry_tests__property_assignments )
{
	log_entry _log_entry;

	// declare resources
	const std::string first_namespace = "inglenook.test.logging";
	const std::string second_namespace = "logging.test.inglenook";
	const std::string third_namespace = "";

	const std::string first_message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras lacinia porta quam quis congue.";
	const std::string second_message = "Ut et purus orci. Donec id dolor nulla, eget aliquet augue. Duis ut turpis interdum justo adipiscing tempus et in diam. Morbi malesuada dapibus euismod. Aliquam consectetur venenatis leo, et pretium ligula semper suscipit. Ut ultricies iaculis fermentum. Vestibulum porttitor, est tristique laoreet porttitor, arcu felis lobortis metus, a hendrerit ligula sapien quis ante. Vivamus feugiat lacus non purus mattis vehicula. In felis urna, pulvinar id cursus fringilla, porttitor in augue. Nam turpis velit, dictum sit amet suscipit quis, aliquet id ligula. In hendrerit tellus vel nunc interdum ultrices. Vestibulum volutpat erat at erat accumsan dignissim. Nam posuere tincidunt lacus eu tincidunt. ";
	const std::string third_message = "";

	const std::string first_key = "key 1";
	const std::string second_key = "key 2";
	const std::string third_key = "key 3";
	const std::string first_value = "value 1";
	const std::string second_value = "value 2";
	const std::string third_value_a = "value 3a";
	const std::string third_value_b = "value 3b";

	// check assignment and recall of entry_type property
	_log_entry.entry_type(category::debugging);			BOOST_CHECK(_log_entry.entry_type() == category::debugging);
	_log_entry.entry_type(category::verbose);			BOOST_CHECK(_log_entry.entry_type() == category::verbose);
	_log_entry.entry_type(category::information);		BOOST_CHECK(_log_entry.entry_type() == category::information);
	_log_entry.entry_type(category::warning);			BOOST_CHECK(_log_entry.entry_type() == category::warning);
	_log_entry.entry_type(category::error);				BOOST_CHECK(_log_entry.entry_type() == category::error);
	_log_entry.entry_type(category::fatal);				BOOST_CHECK(_log_entry.entry_type() == category::fatal);

	// check assignment and recall of log_namespace property
	_log_entry.log_namespace(first_namespace); 			BOOST_CHECK(_log_entry.log_namespace() == first_namespace);
	_log_entry.log_namespace(second_namespace);			BOOST_CHECK(_log_entry.log_namespace() == second_namespace);
	_log_entry.log_namespace(third_namespace);			BOOST_CHECK(_log_entry.log_namespace() == third_namespace);

	// check assignment and recall of message property
	_log_entry.message(first_message); 					BOOST_CHECK(_log_entry.message() == first_message);
	_log_entry.message(second_message);					BOOST_CHECK(_log_entry.message() == second_message);
	_log_entry.message(third_message);					BOOST_CHECK(_log_entry.message() == third_message);

	// test data addition (test items are acually inserted).
	_log_entry.extended_data(first_key, first_value);	BOOST_CHECK(_log_entry.extended_data().size() == 1);
	_log_entry.extended_data(second_key, second_value);	BOOST_CHECK(_log_entry.extended_data().size() == 2);
	_log_entry.extended_data(third_key, third_value_a);	BOOST_CHECK(_log_entry.extended_data().size() == 3);
	// should reassign third_key to third_value_b, not create a new key
	_log_entry.extended_data(third_key, third_value_b);	BOOST_CHECK(_log_entry.extended_data().size() == 3);

	// ensure data elements are as expected
	BOOST_CHECK(_log_entry.extended_data().find(first_key)->second == first_value);
	BOOST_CHECK(_log_entry.extended_data().find(second_key)->second == second_value);
	BOOST_CHECK(_log_entry.extended_data().find(third_key)->second == third_value_b);

	// test data removal
	_log_entry.extended_data(first_key, "");	BOOST_CHECK(_log_entry.extended_data().size() == 2);
	// should not remove anything as this key has already been removed.
	_log_entry.extended_data(first_key, "");	BOOST_CHECK(_log_entry.extended_data().size() == 2);
	_log_entry.extended_data(second_key, "");	BOOST_CHECK(_log_entry.extended_data().size() == 1);
	_log_entry.extended_data(third_key, "");	BOOST_CHECK(_log_entry.extended_data().size() == 0);

}

} // namespace inglenook::logging

} // namespace inglenook
