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
#include "application_program_options.h"

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
    /// Cache of the application's description.
    std::string m_description("");

    /// Cache of the application's version.
    std::string m_version("");

    /// Cache of the application's build number.
    std::string m_build("");

    /// Cache of the application's config file.
    boost::filesystem::path m_config_file("");

    /// Cache of the application's config arguments.
    std::map<std::string, std::string> m_config_arguments;

    /// Cache of the application's 'process id'.
    pid_type m_pid(0);

    /// Cache of the application's 'process name'.
    std::string m_name("");

    /**
     * Custom parser to extract config arguments from the command line.
     * @param raw_argument The raw argument to be parsed.
     * @return the key/value pair if it matches the required prefix, otherwise just an empty pair.
     */
    std::pair<std::string, std::string> config_arguments_parser(const std::string& raw_argument)
    {
        // Set the default return value to a blank pair, this indicates that this argument is not for us.
        std::pair<std::string, std::string> return_parsed;
        
        // Does the specified argument start with the config command line prefix?
        if(raw_argument.find(PO_CONFIG_COMMAND_PREFIX) == 0)
        {
            // Fetch the key=value part of the argument string.
            std::string keyValue = raw_argument.substr(PO_CONFIG_COMMAND_PREFIX.length());
            
            // We need to extract the key and value, which should be separated use the '=' character.
            size_t equalsPosition = keyValue.find('=');
            
            // Did we find the '=' separator.
            if(equalsPosition != std::string::npos)
            {
                // Add the split key and value to the map.
                m_config_arguments[keyValue.substr(0, equalsPosition)] = keyValue.substr(equalsPosition + 1);
                
                // We have to return the ignore option to stop the arguments parser from failing, store the key/value in its vector.
                return_parsed = std::make_pair(PO_IGNORE_CONFIG_COMMAND, keyValue);
            }
            else
            {
                // Does not match expected key=value format, ignore.
            }
        }
        
        // Return the result of our parsing.
        return return_parsed;
    }
}
//--------------------------------------------------------//

//--------------------------------------------------------//
application::application(const std::string& description, const std::string& version, const std::string& build_date, const std::string& build_time)
{
    // Store the required information.
    m_description = description;
    m_version = version;
    m_build = build_date + " " + build_time;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
application::~application()
{
    // Nothing to do here.
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string application::description()
{
    // Return the description.
    return m_description;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string application::version()
{
    // Return the version number.
    return m_version;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string application::build()
{
    // Return the build information.
    return m_build;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
boost::filesystem::path application::config_file()
{
    // Return the config file.
    return m_config_file;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
void application::config_file(const boost::filesystem::path& config_file)
{
    // Set the config file.
    m_config_file = config_file;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
const std::map<std::string, std::string>& application::config_arguments()
{
    // Return the config arguments map.
    return m_config_arguments;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
void application::config_arguments(const std::map<std::string, std::string>& config_arguments)
{
    // Set the config arguments map.
    m_config_arguments = config_arguments;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
pid_type application::pid()
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
    // Have we already fetched the value?
    if(m_name.empty())
    {
        // Shortly we'll delve in to some platform specific stuff. these blocks are designed such that
        // they should place a path to this processes entry assembly (relative or absolute) in to this boost
        // path object. we will then trim this to get the assembly name.
        boost::filesystem::path binary_path("");
        
    #if defined(__linux__) // the following block is for LINUX and is both tested and maintained.
        
        // Build path to the cmdline file for this process.
        boost::filesystem::path application_cmdline_file_path((boost::format("/proc/%1%/cmdline") % pid() ).str());
        
        try
        {
            // Make sure this file actually exists before proceeding.
            if(boost::filesystem::exists(application_cmdline_file_path))
            {
                // File exists... try and open it.
                std::ifstream application_cmdline_file(application_cmdline_file_path.native().c_str());
                
                // Check if we managed to open the file, throw exception on failure.
                if(!application_cmdline_file.is_open())
                {
                    BOOST_THROW_EXCEPTION(exceptions::application_name_exception());
                }
                
                // Create commandLine and copy file contents in to it. DO NOT remove braces around the
                // first argument - c++11 resolution for potential most-vexing-parse issues
                std::string command_line(
                                         { std::istreambuf_iterator<char>(application_cmdline_file) },
                                         std::istreambuf_iterator<char>()
                                         );
                
                // Create a string stream to delimit the file.
                std::stringstream parser(command_line);
                std::string buffer;
                
                // Read a line in to the buffer, and then convert to path.
                std::getline(parser, buffer, '\0');
                binary_path = buffer;
                
            }
            // The cmdline file doesn't appear to exist?
            else
            {
                BOOST_THROW_EXCEPTION(exceptions::application_name_exception());
            }
        }
        catch(boost::exception& ex)
        {
            // Unknown error.
            ex << exceptions::application_cmdline_file(application_cmdline_file_path);
            throw;
        }
        
    #elif defined(__APPLE__) // the following block is for OSX and is both tested and maintained.
        
        // Define a buffer to store path.
        char application_name[PROC_PIDPATHINFO_MAXSIZE] = {};
        
        // Attempt to fetch the process path using the pid.
        if(proc_pidpath(pid(), application_name, sizeof(application_name)) <= 0)
        {
            // Failed to get process path!
            BOOST_THROW_EXCEPTION(exceptions::application_name_exception()
                                  << exceptions::c_error_number(errno)
                                  << exceptions::c_error_message(strerror(errno))
                                  );
        }
        else
        {
            // We have the process name.
            binary_path = application_name;
        }
        
    #elif defined(_WIN32) // The following block is for WINDOWS and is NOT tested or maintained.
        #warning INGLENOOK: WIN32 code has never been tested. This might not even compile. Good luck brave warrior.
        
        // Define a buffer to store path.
        char application_name[MAX_PATH] = {};
        
        try
        {
            // Attempt to open the our own process with read access rights.
            HANDLE application_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, get_real_process_id());
            
            // Check if anything went wrong.
            if(!application_handle)
            {
                BOOST_THROW_EXCEPTION(exceptions::application_name_exception());
            }
            
            // Attempt to open handle was successful, set up a scoped exit clause to
            // ensure that the process handle is closed when we are done.
            BOOST_SCOPE_EXIT((&application_handle))
            {
                CloseHandle(application_handle);
            } BOOST_SCOPE_EXIT_END
            
            // Query the name of the process using the handle acquired.
            if(!GetModuleFileNameEx(application_handle, 0, application_name, sizeof(application_name) - 1))
            {
                // Something went wrong, abort and sulk
                BOOST_THROW_EXCEPTION(exceptions::application_name_exception() << exceptions::win32_error_number(GetLastError()));
            }
            
            // We have the process name.
            binary_path = application_name;
        }
        catch(boost::exception& ex)
        {
            // In the event of any errors note that this is windows code.
            ex << exceptions::is_win32_error(true);
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

//--------------------------------------------------------//
bool application::arguments_parser(boost::program_options::variables_map& variables_map, const int& argc, const char* argv[], const boost::program_options::options_description& options, const boost::program_options::positional_options_description& positions)
{
    // Keep track of whether we should suggest exit the application.
    bool should_exit(false);

    // Generic options (help, version and config file).
    boost::program_options::options_description generic(boost::locale::translate("Other options").str().c_str());
    generic.add_options()
        (PO_HELP_FULL.c_str(), boost::locale::translate("produce help message").str().c_str())
        (PO_VERSION_FULL.c_str(), boost::locale::translate("produce version information").str().c_str())
        (PO_CONFIG_FILE_FULL.c_str(), boost::program_options::value<std::string>(), boost::locale::translate("override the global/application configuration file").str().c_str())
    ;

    // Setup the program options, this joins the options together and sets the program description.
    boost::program_options::options_description cmdline_options(name() + ", " + description());

    // Have any additional options been specified?
    if(options.options().size() > 0)
    {
        // Add the additional options first.
        cmdline_options.add(options);
    }

    // Add the generic options.
    cmdline_options.add(generic);

    // Add hidden options (ignore-config-command).
    boost::program_options::options_description all_options;
    all_options.add_options()
        (PO_IGNORE_CONFIG_COMMAND.c_str(), boost::program_options::value< std::vector<std::string> >(), "")
    ;
    all_options.add(cmdline_options);

    try
    {
        // Parse the arguments!
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(all_options).positional(positions).extra_parser(config_arguments_parser).run(), variables_map);
        
        // Did the user request help?
        if(variables_map.count(PO_HELP_FULL))
        {
            // Print out the available options.
            std::cout << cmdline_options;
            
            // Suggest that the application should exit.
            should_exit = true;
        }
        // Did the user request version information?
        else if(variables_map.count(PO_VERSION_FULL))
        {
            // Print out the version information.
            std::cout << name() << " " << version() << " (compiled " << build() << ")" << std::endl << std::endl;
            
            std::cout << "Copyright (C) 2012, Project Inglenook (http://www.project-inglenook.co.uk)" << std::endl << std::endl;
            
            std::cout << "This program is free software: you can redistribute it and/or modify" << std::endl
            << "it under the terms of the GNU General Public License as published by" << std::endl
            << "the Free Software Foundation, either version 3 of the License, or" << std::endl
            << "(at your option) any later version." << std::endl << std::endl;
            
            std::cout << "This program is distributed in the hope that it will be useful," << std::endl
            << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl
            << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the" << std::endl
            << "GNU General Public License for more details." << std::endl << std::endl;
            
            std::cout << "You should have received a copy of the GNU General Public License" << std::endl
            << "along with this program. If not, see <http://www.gnu.org/licenses/>." << std::endl;
            
            // Suggest that the application should exit.
            should_exit = true;
        }
        else
        {
            // Perform the final validation of the other options, will throw if there are issues.
            boost::program_options::notify(variables_map);
        }
        
        // Did the user specify a config file.
        if(variables_map.count(PO_CONFIG_FILE_FULL))
        {
            // Set the specified config file.
            config_file(variables_map[PO_CONFIG_FILE_FULL].as<std::string>());
        }
    }
    catch(std::exception &ex)
    {
        // Print out help to remind them.
        std::cout << cmdline_options << std::endl;
        
        // Failed to parse arguments!
        std::cerr << boost::locale::translate("ERROR: ") << ex.what() << std::endl;
        
        // Throw that there has been an error.
        BOOST_THROW_EXCEPTION(exceptions::application_arguments_parser_exception());
    }
    
    // Return the exit suggestion.
    return should_exit;
}
//--------------------------------------------------------//
