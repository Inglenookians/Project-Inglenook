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

namespace inglenook
{
    namespace core
    {  
        namespace application
        {
            
            /**
             * Fetch the application's process id (pid).
             * @return The application's pid.
             */
            pid_t pid();
            
            /**
             * Fetch the application's process name.
             * @return The application's process name.
             */
            std::string name();
            
            
            /// @todo add a default command line options parser (for help/version/etc...)
            
        }
    }
}
