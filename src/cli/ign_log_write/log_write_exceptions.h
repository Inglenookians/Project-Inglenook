/*
* log_exceptions.cpp: ign_log_write exception constants and structures..
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

// standard library includes
#include <exception>

// boost (http://boost.org) includes
#include <boost/exception/all.hpp>
#include <boost/locale.hpp>

namespace inglenook
{

namespace logging
{

/// base of all logging exceptions
const unsigned long module_error_base = 0xff10000;

//
// log_write_exception
// Standard base exception for all exceptions thrown by ign_log_write.
struct log_write_exception: virtual boost::exception, virtual std::exception
{/// provides a boiler plate explanation of the exception.
   const char* what() const throw() {
	   return boost::locale::translate("Unhandled exception in CLI log writer (ign_log_write)").str().c_str();
   }
};

} // namespace inglenook::logging

} // namespace inglenook
