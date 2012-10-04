/*
* logging_test.h: Test routines to make sure the logging mechanisms are working.
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

#ifndef LOGGING_TESTS_
#define LOGGING_TESTS_


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE common_logging

// boost (http://boost.org) includes
#include <boost/test/unit_test.hpp>

namespace inglenook
{

namespace logging
{

BOOST_AUTO_TEST_CASE ( log_writer_tests )
{
	// pending cmake
}

BOOST_AUTO_TEST_CASE ( log_client_tests )
{
	// pending cmake
}

BOOST_AUTO_TEST_CASE ( log_entry )
{
	// pending cmake
}



} // namespace inglenook::logging

} // namespace inglenook

#endif LOGGING_TESTS_
