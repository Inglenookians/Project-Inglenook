#pragma once

/*
 * application.h: Application specific details.
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

//
// The following pre-compiler should:
// create a new type definition of 'pid_type' which is appropriate to store the platform specific process identifier.
// create a new definition of 'INGLENOOK_CURRENT_PID' which can be invoked to get the current process id.
// Example:
// // get and store current process id.
// pid_type mypid = INGLENOOK_CURRENT_PID();
//
// Linux and OSX support (officially tested and maintained).
#if defined(__linux__) || defined(__APPLE__)

    #include <sys/types.h>
    #include <unistd.h>
    #include <stdlib.h>
    #define INGLENOOK_CURRENT_PID ( getpid )
    typedef pid_t pid_type;

    // OSX needs additional header for fetching the process name.
    #if defined(__APPLE__)
        #include <libproc.h>
    #endif

// Windows support (not tested or maintained).
#elif defined(_WIN32)

    # include <windows.h>
    # warning INGLENOOK: WIN32 code has never been tested. This might not even compile. Good luck brave warrior.
    # define INGLENOOK_CURRENT_PID ( GetCurrentProcessId )
    typedef DWORD pid_type;

#endif

// standard library includes
#include <string>

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace inglenook
{
    namespace core
    {  
        class application
        {
        public:
            
            /**
             * Constructor.
             * @param description The application description.
             * @param version The application version.
             * @param build_date The build date of the application.
             * @param build_time The build time of the application.
             */
            application(const std::string& description, const std::string& version, const std::string& build_date, const std::string& build_time);
            
            /**
             * Deconstructor.
             */
            ~application();
            
            /**
             * Fetch the application description.
             * @retutn the description.
             */
            static std::string description();
            
            /**
             * Fetch the application version number.
             * @retutn the version number.
             */
            static std::string version();
            
            /**
             * Fetch the application build information.
             * @retutn the build information.
             */
            static std::string build();
            
            /**
             * Fetch the application overriding config file, which can be set 
             * using the default argument parser as well as manually.
             * @retutn the config file.
             */
            static boost::filesystem::path config_file();
            
            /**
             * Set the overriding config file for the application.
             * @param config_file The config file to set.
             */
            static void config_file(const boost::filesystem::path& config_file);
            
            /**
             * Fetch the application's process id (pid).
             * @return The application's pid.
             */
            static pid_t pid();
            
            /**
             * Fetch the application's process name.
             * @return The application's process name.
             */
            static std::string name();
            
            /**
             * Parse the command line arguments with the default help, version and config-file options.
             * @param argc The argument count.
             * @param argv The argument vector.
             * @param options The additional program options to parse.
             * @param positions The specified positions for options that don't have to be named.
             * @return The parsed arguments as a variable map.
             * @note This will throw if the help or version options are specified to try and stop the application loop from continuing.
             */
            static boost::program_options::variables_map arguments_parser(int argc, char* argv[], const boost::program_options::options_description& options = boost::program_options::options_description(), const boost::program_options::positional_options_description& positions = boost::program_options::positional_options_description());
            
        };
        
    }
}
