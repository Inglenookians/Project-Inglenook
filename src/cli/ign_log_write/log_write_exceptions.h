#pragma once
/*
* log_write_exceptions.h: ign_log_write exception constants and structures..
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

// standard library includes
#include <exception>

// boost (http://boost.org) includes
#include <boost/exception/all.hpp>
#include <boost/locale.hpp>

// inglenook includes
#include <ign_core/exceptions.h>

namespace inglenook
{

namespace logging
{

/// base of all logging exceptions
const unsigned long module_error_base = 0xff10000;

/// an action expected a parameter that wasn't provided.
const unsigned long log_write_exception_missing_argument = module_error_base + 0x01;

/// argument that was expected, but not provided.
typedef boost::error_info<struct __expected_argument, std::string> expected_argument;

/// couldn't work out how to use provided arguments - this is what was expected.
typedef boost::error_info<struct __acceptable_arguments, std::string> acceptable_arguments;

/// couldn't work out how to use provided arguments - this is what was expected.
typedef boost::error_info<struct __unrecongised_action, std::string> unrecognised_action;

/**
 * Standard base exception for all exceptions thrown by ign_log_write.
 */
struct log_write_exception: virtual inglenook::core::exceptions::inglenook_exception
{	
	/// provides a boiler plate explaination of the exception.
   const char* what() const throw() {
	   return boost::locale::translate("Unhandled exception in CLI log writer (ign_log_write)").str().c_str();
   }
};

/**
 * Thrown when an action requires a parameter that has not been provided.
 */
struct action_required_arguments_missing_exception: virtual log_write_exception
{
	/// provides a boiler plate explaination of the exception.
   const char* what() const throw() {
	   return boost::locale::translate("An argument required by this action was not specified.").str().c_str();
   }
};

/**
 * Throw when the wrong number of arguments is received by an action processor
 */
struct wrong_number_of_arguments_exception: virtual log_write_exception
{
	/// provides a boiler plate explaination of the exception.
   const char* what() const throw() {
	   return boost::locale::translate("The action invoked couldn't work out how to use to arguments provided.").str().c_str();
   }
};

/**
 * Thrown when an action is provided that is not understood by the software.
 */
struct unrecognised_action_exception: virtual log_write_exception
{
	/// provides a boiler plate explaination of the exception.
   const char* what() const throw() {
	   return boost::locale::translate("The action was not understood.").str().c_str();
   }
};


} // namespace inglenook::logging

} // namespace inglenook
