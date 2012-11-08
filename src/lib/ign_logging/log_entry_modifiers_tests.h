/*
* logging_entry_modifiers_test.h: Test routines for the log stream modifier classes (logging_entry_modifiers.h)
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

#pragma once

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LOG_TEST_NAME

// boost (http://boost.org) includes
#include <boost/test/unit_test.hpp>

// inglenook includes
#include "log_entry_modifiers.h"

namespace inglenook
{

namespace logging
{

//
// log_entry_tests__defaults
// checks all the default values for correctness. This should ensure that
// the default values are predictable, and that the read accessors appear
// to be working as expected (these will be further tested).
BOOST_AUTO_TEST_CASE ( log_entry_modifiers_tests__construction )
{

	// resources
	const std::string ns_test_value = "inglenook.logging.test";
	const std::string log_data_test_key = "inglenook.key";
	const std::string log_data_test_value = "a value";
	
	// check ns construction 
	ns _ns(ns_test_value);
	BOOST_CHECK(_ns.m_ns == ns_test_value);

	// check log_data construction
	log_data _log_data(log_data_test_key, log_data_test_value);
	BOOST_CHECK(_log_data.m_key == log_data_test_key);
	BOOST_CHECK(_log_data.m_value == log_data_test_value);

}

} // namespace inglenook::logging

} // namespace inglenook
