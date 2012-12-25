#pragma once
/*
* log_entry_buffered_tests.h: Test routines for the log_entry_buffered class (log_entry_buffered.cpp/h)
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
#include "log_entry_buffered.h"

namespace inglenook
{

namespace logging
{

//
// log_entry_tests__defaults
// checks all the default values for correctness. This should ensure that
// the default values are predictable, and that the read accessors appear
// to be working as expected (these will be further tested).
BOOST_AUTO_TEST_CASE ( log_entry_buffered_tests__defaults )
{
    log_entry_buffered _log_entry_buffered;

    // check default values
    BOOST_CHECK(_log_entry_buffered.message() == "");
    BOOST_CHECK(_log_entry_buffered.message_buffer().str() == "");

    //
    // all other property assignments are tested in the parent class(es) tests.
    //
}

BOOST_AUTO_TEST_CASE ( log_entry_buffered_tests__property_assignments )
{
    log_entry_buffered _log_entry_buffered;

    const std::string message_component_1 = "The meaning of life is ";
    const std::string message_component_2 = "defined by the number: ";
    const std::string message_component_3 = "42";
    const int message_component_3_int = 42;

    const std::string message_component_4 = "Hello ";
    const std::string message_component_5 = "world!";

    // check that the stream operator is working as expected.
    _log_entry_buffered.message_buffer() << message_component_1 << message_component_2;
    BOOST_CHECK(_log_entry_buffered.message_buffer().str() == message_component_1 + message_component_2); 
    BOOST_CHECK(_log_entry_buffered.message() == message_component_1 + message_component_2);

    // check stream is still working after read calls to message().
    _log_entry_buffered.message_buffer() << message_component_3_int;
    BOOST_CHECK(_log_entry_buffered.message_buffer().str() == message_component_1 + message_component_2 + message_component_3); 
    BOOST_CHECK(_log_entry_buffered.message() == message_component_1 + message_component_2 + message_component_3);

    // check assignment using the message() variable works
    _log_entry_buffered.message(message_component_4);
    BOOST_CHECK(_log_entry_buffered.message_buffer().str() == message_component_4); 
    BOOST_CHECK(_log_entry_buffered.message() == message_component_4);

    // check stream is still working after write calls to message().
    _log_entry_buffered.message_buffer() << message_component_5;
    BOOST_CHECK(_log_entry_buffered.message_buffer().str() == message_component_4 + message_component_5); 
    BOOST_CHECK(_log_entry_buffered.message() == message_component_4 + message_component_5);
}

} // namespace inglenook::logging

} // namespace inglenook
