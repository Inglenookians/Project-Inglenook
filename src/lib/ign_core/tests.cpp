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

// boost includes
#include <boost/test/unit_test.hpp>

// inglenook includes
#include "application.h"
#include "environment.h"

// application test case.
BOOST_AUTO_TEST_CASE(application)
{
    // Test the application name (we know this!)
    BOOST_CHECK(inglenook::core::application::name() == "ign_core_tests");
    
    // It's difficult to test whether the pid is correct, just check a valid pid is returned (ie: pid > 0)
    BOOST_CHECK(inglenook::core::application::pid() > 0);
    
    /// @todo test for program options.
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
    BOOST_CHECK(inglenook::core::environment::set(variable_one, "ONE", true) == true);
    BOOST_CHECK(inglenook::core::environment::get(variable_one) == "ONE");
    BOOST_CHECK(inglenook::core::environment::set(variable_one, "TWO", true) == true);
    BOOST_CHECK(inglenook::core::environment::get(variable_one) != "ONE");
    BOOST_CHECK(inglenook::core::environment::get(variable_one) == "TWO");
    
    // Test setting with overwrite disabled for pre-existing variable.
    BOOST_CHECK(inglenook::core::environment::set(variable_one, "THREE", false) == true);
    BOOST_CHECK(inglenook::core::environment::get(variable_one) != "ONE");
    BOOST_CHECK(inglenook::core::environment::get(variable_one) == "TWO");
    BOOST_CHECK(inglenook::core::environment::get(variable_one) != "THREE");
    
    // Test setting with overwrite disabled for new variable.
    BOOST_CHECK(inglenook::core::environment::set(variable_two, "APPLE", false) == true);
    BOOST_CHECK(inglenook::core::environment::get(variable_two) == "APPLE");
    BOOST_CHECK(inglenook::core::environment::set(variable_two, "ORANGE", false) == true);
    BOOST_CHECK(inglenook::core::environment::get(variable_two) == "APPLE");
    BOOST_CHECK(inglenook::core::environment::get(variable_two) != "ORANGE");
}
