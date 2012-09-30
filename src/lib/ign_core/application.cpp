/*
 * application.cpp: Application specific details.
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

#include "application.h"

// inglenook includes
#include "application_exceptions.h"

// standard library includes
#include <fstream>
#include <sstream>

// boost (http://boost.org) includes
#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/locale.hpp>
#include <boost/scope_exit.hpp>

using namespace inglenook::core;

//--------------------------------------------------------//
namespace
{
    /// Cache of process information so we only fetch it once.
    pid_t m_pid(0);
    std::string m_name("");
}
//--------------------------------------------------------//

//--------------------------------------------------------//
pid_t application::pid()
{
    // Have we already fetched the value?
    if(m_pid == 0)
    {   
        // Fetch the process id for the current process using the platform specific method call and cache it.
        m_pid = INGLENOOK_CURRENT_PID();
    }
    
    // Return the process id.
    return m_pid;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string application::name()
{
    /// @todo Investigate whether this should raise a log entry.
    
    // Have we already fetched the value?
    if(m_name.empty())
    {
        // Shortly we'll delve in to some platform specific stuff. these blocks are designed such that
        // they should place a path to this processes entry assembly (relative or absolute) in to this boost
        // path object. we will then trim this to get the assembly name.
        boost::filesystem::path binary_path("");
        
    #if defined(__linux__) // the following block is for LINUX and is both tested and maintained.
        
        // Build path to the cmdline file for this process.
        boost::filesystem::path process_cmdline_file_path((boost::format("/proc/%1%/cmdline") % pid() ).str());
        
        try
        {
            // Make sure this file actually exists before proceeding.
            if(boost::filesystem::exists(process_cmdline_file_path))
            {
                // File exists... try and open it.
                std::ifstream process_cmdline_file(process_cmdline_file_path.native().c_str());
                
                // Check if we managed to open the file, throw exception on failure.
                if(!process_cmdline_file.is_open())
                {
                    BOOST_THROW_EXCEPTION(application::exceptions::process_name_exception());
                }
                
                // We have opened the file, set up a scoped exit clause to ensure the file is closed.
                BOOST_SCOPE_EXIT((&process_cmdline_file))
                {
                    process_cmdline_file.close();
                } BOOST_SCOPE_EXIT_END
                
                // Create commandLine and copy file contents in to it. DO NOT remove braces around the
                // first argument - c++11 resolution for potential most-vexing-parse issues
                std::string command_line(
                                         { std::istreambuf_iterator<char>(process_cmdline_file) },
                                         std::istreambuf_iterator<char>()
                                         );
                
                // Create a string stream to delimit the file.
                std::stringstream parser(command_line);
                std::string buffer;
                
                // Read a line in to the buffer, and then convert to path.
                std::getline(parser, buffer, ' ');
                binary_path = buffer;
                
            }
            // The cmdline file doesn't appear to exist?
            else
            {
                BOOST_THROW_EXCEPTION(application::exceptions::process_name_exception());
            }
        }
        catch(boost::exception& ex)
        {
            // Unknown error.
            ex << application::exceptions::process_cmdline_file(process_cmdline_file_path);
            throw;
        }
        
    #elif defined(__APPLE__) // the following block is for OSX and is both tested and maintained.
        
        // Define a buffer to store path.
        char process_name[PROC_PIDPATHINFO_MAXSIZE] = {};
        
        // Attempt to fetch the process path using the pid.
        if(proc_pidpath(pid(), process_name, sizeof(process_name)) <= 0)
        {
            // Failed to get process path!
            BOOST_THROW_EXCEPTION(application::exceptions::process_name_exception()
                                  << core::exceptions::c_error_number(errno)
                                  << core::exceptions::c_error_message(strerror(errno))
                                  );
        }
        else
        {
            // We have the process name.
            binary_path = process_name;
        }
        
    #elif defined(_WIN32) // The following block is for WINDOWS and is NOT tested or maintained.
        #warning INGLENOOK: WIN32 code has never been tested. This might not even compile. Good luck brave warrior.
        
        // Define a buffer to store path.
        char process_name[MAX_PATH] = {};
        
        try
        {
            // Attempt to open the our own process with read access rights.
            HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, get_real_process_id());
            
            // Check if anything went wrong.
            if(!process_handle)
            {
                BOOST_THROW_EXCEPTION(application::exceptions::process_name_exception());
            }
            
            // Attempt to open handle was successful, set up a scoped exit clause to
            // ensure that the process handle is closed when we are done.
            BOOST_SCOPE_EXIT((&process_handle))
            {
                CloseHandle(process_handle);
            } BOOST_SCOPE_EXIT_END
            
            // Query the name of the process using the handle acquired.
            if(!GetModuleFileNameEx(process_handle, 0, process_name, sizeof(process_name) - 1))
            {
                // Something went wrong, abort and sulk
                BOOST_THROW_EXCEPTION(application::exceptions::process_name_exception() << core::exceptions::win32_error_number(GetLastError()));
            }
            
            // We have the process name.
            binary_path = process_name;
        }
        catch(boost::exception& ex)
        {
            // In the event of any errors note that this is windows code.
            ex << core::exceptions::is_win32_error(true);
            throw;
        }
        
    #else // Unsupported platform
        #error INGLENOOK: Unsupported platform.
    #endif
        
        // Hopefully we have a path to pass back by now (empty string on failure).
        m_name = binary_path.filename().native();
    }
    
    // Return the name.
    return m_name;
}
//--------------------------------------------------------//
