/*
 * directories.cpp: Retrieve common directory paths.
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

#include "directories.h"

// inglenook includes
#include <ign_core/environment.h>
#include <ign_core/environment_variables.h>

// boost (http://boost.org) includes
#include <boost/format.hpp>
#include <boost/locale.hpp>

using namespace inglenook;

//--------------------------------------------------------//
// Hide the implementation in the anonynous namespace.
namespace
{

	/**
	 * Implamentation of logic that determines where to find a given inglenook directory.
	 * @param environment_variable environment variable to examine for an value that overrides the default.
	 * @param default_value default value to assume if a specific environment variable is not set.
	 * @param explain if true, will print out logic used to determine the location to standard output.
	 * @returns path that inglenook believes should resolve this query.
	 */
    const boost::filesystem::path implementation(const std::string& environment_variable, const boost::filesystem::path& default_value, bool explain)
    {
        // Fetch the environment variable first.
        boost::filesystem::path return_value(core::environment::get(environment_variable));
        
        // Check environment variable value is set.
        if(return_value.empty())
        {
            // It's not, we therefore return the system default value.
            return_value = default_value;
            
            // Do we need to explain that the system default was selected.
            if(explain)
            {
                std::cout << boost::locale::translate("Directory set using system default") << std::endl;
            }
        }
        else
        {
            // Do we need to explain that the environment variable was selected.
            if(explain)
            {
                std::cout << boost::format(boost::locale::translate("Directory set using environment variable '%1%'")) % environment_variable << std::endl;
            }
        }
        
        // Return the value.
        return return_value;
    }
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const boost::filesystem::path directories::cli(bool explain)
{
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
    
    // Return the cli directory.
    return implementation(core::environment::variables::DIR_CLI, default_path, explain);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const boost::filesystem::path directories::config(bool explain)
{
    // Set the default directory based on current platform.
#if defined(__linux__) || defined(__APPLE__)
    // Linux and OSX support (officially tested and maintained).
    boost::filesystem::path default_path("/etc/inglenook");
#elif defined(_WIN32)
    // Windows support (not tested or maintained).
    #error Default local user directory for Windows not defined.
    boost::filesystem::path default_path("");
#else // Unsupported platform
    #error INGLENOOK: Unsupported platform.
#endif
    
    // Return the config directory.
    return implementation(core::environment::variables::DIR_CONFIG, default_path, explain);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const boost::filesystem::path directories::data(bool explain)
{
    // Set the default directory based on current platform.
#if defined(__linux__) || defined(__APPLE__)
    // Linux and OSX support (officially tested and maintained).
    boost::filesystem::path default_path("/var/lib/inglenook");
#elif defined(_WIN32)
    // Windows support (not tested or maintained).
    #error Default local user directory for Windows not defined.
    boost::filesystem::path default_path("");
#else // Unsupported platform
    #error INGLENOOK: Unsupported platform.
#endif
    
    // Return the data directory.
    return implementation(core::environment::variables::DIR_DATA, default_path, explain);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const boost::filesystem::path directories::lib(bool explain)
{
    // Set the default directory based on current platform.
#if defined(__linux__) || defined(__APPLE__)
    // Linux and OSX support (officially tested and maintained).
    boost::filesystem::path default_path("/usr/local/lib");
#elif defined(_WIN32)
    // Windows support (not tested or maintained).
    #error Default local user directory for Windows not defined.
    boost::filesystem::path default_path("");
#else // Unsupported platform
    #error INGLENOOK: Unsupported platform.
#endif
    
    // Return the lib directory.
    return implementation(core::environment::variables::DIR_LIB, default_path, explain);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const boost::filesystem::path directories::log(bool explain)
{
    // Set the default directory based on current platform.
#if defined(__linux__) || defined(__APPLE__)
    // Linux and OSX support (officially tested and maintained).
    boost::filesystem::path default_path("/var/log/inglenook");
#elif defined(_WIN32)
    // Windows support (not tested or maintained).
    #error Default local user directory for Windows not defined.
    boost::filesystem::path default_path("");
#else // Unsupported platform
    #error INGLENOOK: Unsupported platform.
#endif
    
    // Return the log directory.
    return implementation(core::environment::variables::DIR_LOG, default_path, explain);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const boost::filesystem::path directories::sbin(bool explain)
{
    // Set the default directory based on current platform.
#if defined(__linux__) || defined(__APPLE__)
    // Linux and OSX support (officially tested and maintained).
    boost::filesystem::path default_path("/usr/local/sbin");
#elif defined(_WIN32)
    // Windows support (not tested or maintained).
    #error Default local user directory for Windows not defined.
    boost::filesystem::path default_path("");
#else // Unsupported platform
    #error INGLENOOK: Unsupported platform.
#endif
    
    // Return the sbin directory.
    return implementation(core::environment::variables::DIR_SBIN, default_path, explain);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const boost::filesystem::path directories::man(bool explain)
{
    // Set the default directory based on current platform.
#if defined(__linux__) || defined(__APPLE__)
    // Linux and OSX support (officially tested and maintained).
    boost::filesystem::path default_path("/usr/local/share/man");
#elif defined(_WIN32)
    // Windows support (not tested or maintained).
    #error Default local user directory for Windows not defined.
    boost::filesystem::path default_path("");
#else // Unsupported platform
    #error INGLENOOK: Unsupported platform.
#endif
    
    // Return the man directory.
    return implementation(core::environment::variables::DIR_MAN, default_path, explain);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const boost::filesystem::path directories::tmp(bool explain)
{
    // Set the default directory based on current platform.
#if defined(__linux__) || defined(__APPLE__)
    // Linux and OSX support (officially tested and maintained).
    boost::filesystem::path default_path("/var/tmp/inglenook");
#elif defined(_WIN32)
    // Windows support (not tested or maintained).
    #error Default local user directory for Windows not defined.
    boost::filesystem::path default_path("");
#else // Unsupported platform
    #error INGLENOOK: Unsupported platform.
#endif
    
    // Return the tmp directory.
    return implementation(core::environment::variables::DIR_TMP, default_path, explain);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const boost::filesystem::path directories::user(bool explain)
{
    // Set the default directory based on current platform.
#if defined(__linux__) || defined(__APPLE__)
    // Linux and OSX support (officially tested and maintained).
    boost::filesystem::path default_path(core::environment::get("HOME"));
#elif defined(_WIN32)
    // Windows support (not tested or maintained).
    #error Default local user directory for Windows not defined.
    boost::filesystem::path default_path("");
#else // Unsupported platform
    #error INGLENOOK: Unsupported platform.
#endif
    
    // Return the local user directory.
    return implementation(core::environment::variables::DIR_USER, default_path, explain);
}
//--------------------------------------------------------//
