/*
 * environment.cpp: Store and retrieve environment variable values.
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

#include "environment.h"

// standard library includes
#include <cerrno>
#include <iostream>

// boost (http://boost.org) includes
#include <boost/format.hpp>
#include <boost/locale.hpp>

using namespace inglenook::core;

//--------------------------------------------------------//
std::string environment::get(const std::string& variable, const std::string& default_value)
{
    // Set the default return value.
    auto return_value(default_value);
    
    // Try to fetch the environment variable.
    const char* raw_value = std::getenv(variable.c_str());
    
    // Check whether we was successful.
    if(raw_value != NULL)
    {
        // Sucessful fetch, store the value to be returned.
        return_value = std::string(raw_value);
    }
    
    // Return the value.
    return return_value;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
bool environment::set(const std::string& variable, const std::string& value, bool overwrite)
{
    // Keep track of the success.
    auto success = false;
    
    // Try to set the environment variable.
    if(setenv(variable.c_str(), value.c_str(), overwrite) == 0)
    {
        // Success!
        success = true;
    }
    else
    {
        // Error trying to set environment variable.
        /// @todo Investigate whether this should raise a log entry.
        /// @todo Should this raise an exception instead of returning a boolean.
        std::cerr << boost::format(boost::locale::translate("Error trying to set environment variable '%1%' to '%2%': %3%")) % variable % value % std::strerror(errno) << std::endl;
    }
    
    // Return our sucess.
    return success;
}
//--------------------------------------------------------//
