/*
 * tests.cpp: Tests for the ign_directories library.
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
#define BOOST_TEST_MODULE ign_directories_tests

// boost includes
#include <boost/test/unit_test.hpp>

// inglenook includes
#include "directories.h"
#include <ign_core/environment.h>
#include <ign_core/environment_variables.h>

// directories test case.
BOOST_AUTO_TEST_CASE(directories)
{
    // Ensure the DIR_CLI environment variable is not set first.
    inglenook::core::environment::set(inglenook::core::environment::variables::DIR_CLI, "");

    // Set the default directory based on current platform.
#if defined(__linux__) || defined(__APPLE__)
    // Linux and OSX support (officially tested and maintained).
    boost::filesystem::path default_path("/usr/local/bin");
#elif defined(_WIN32)
    // Windows support (not tested or maintained).
#error Default local user directory for Windows not defined.
    boost::filesystem::path default_path("");
#else // Unsupported platform
#error INGLENOOK: Unsupported platform.
#endif
    
    // Check the system default path is returned.
    BOOST_CHECK(inglenook::directories::cli() == default_path);
    
    // Set the DIR_CLI environment variable.
    inglenook::core::environment::set(inglenook::core::environment::variables::DIR_CLI, "test");
    
    // Check the DIR_CLI environment variable value is returned.
    BOOST_CHECK(inglenook::directories::cli() != default_path);
    BOOST_CHECK(inglenook::directories::cli() == "test");
    
    // Set the DIR_CLI environment variable clear.
    inglenook::core::environment::set(inglenook::core::environment::variables::DIR_CLI, "");
    
    // Check the system default path is returned.
    BOOST_CHECK(inglenook::directories::cli() == default_path);
    BOOST_CHECK(inglenook::directories::cli() != "test");
    
    /// @todo Add checks for each directory type or does the above tests suffice?
}
