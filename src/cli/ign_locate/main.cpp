/*
 * main.cpp: A tool for locating various special inglenook directories.
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

// inglenook includes
#include <ign_core/application.h>
#include <ign_core/application_exceptions.h>
#include <ign_directories/directories.h>

#include <ign_config/config.h>


// standard library includes
#include <iostream>

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <boost/program_options.hpp>

int main(int argc, char* argv[])
{
    // Keep track of our success.
    int success = EXIT_FAILURE;
    
    // Program description.
    std::string description("a tool for locating various special inglenook directories");
    
    // Program version
    /// @todo calculate version from somewhere?
    std::string version("0.0.1a");
    
    // Create the application store.
    inglenook::core::application app(description, version,  __DATE__, __TIME__);
    
    // Program specific options.
    boost::program_options::options_description options("Program options");
    options.add_options()
        ("dir,d", boost::program_options::value<std::string>()->required(),
            "REQUIRED: The directory to locate. Available options are:\n\n"
            "cli     \tThe location of inglenook's command line tools\n"
            "config  \tThe location of inglenook's configuration files\n"
            "data    \tThe location of inglenook's data files\n"
            "lib     \tThe location of inglenook's internal libraries\n"
            "log     \tThe location of inglenook's log files\n"
            "sbin    \tThe location of inglenook's system administrator tools\n"
            "man     \tThe location of inglenook's manual pages\n"
            "tmp     \tThe location of inglenook's temporary files\n"
            "user    \tThe location of user's home directory\n")
        ("switch,s", "Automatically change directory once it is found, equivent of the system call:\n\ncd $(ign_locate directory)\n")
        ("verbose,v", "Detailed output indicate how it came to its conclusion (detailing where it looked to find the directory)")
    ;
    
    // Set the positional options.
    boost::program_options::positional_options_description positions;
    positions.add("dir", 1);
    
    // Try and parse the command line arguments.
    bool parser_exit = false;
    boost::program_options::variables_map vm;
    try
    {
        vm = inglenook::core::application::arguments_parser(argc, argv, options, positions);
    }
    catch(inglenook::core::exceptions::application_exit_success_exception &ex)
    {
        success = EXIT_SUCCESS;
        parser_exit = true;
    }
    catch(inglenook::core::exceptions::application_exit_fail_exception &ex)
    {
        parser_exit = true;
    }
    
    // Has the parser indicated we should continue.
    if(!parser_exit)
    {
        // Keep track of user switches.
        bool option_switch(false);
        bool option_verbose(false);
        std::string option_directory("");
        
        // Did the user request automatic directory switching?
        if(vm.count("switch"))
        {
            option_switch = true;
        }
        // Did the user request verbose output?
        if(vm.count("verbose"))
        {
            option_verbose = true;
        }
        // Did the user specify a directory.
        if(vm.count("dir") > 0)
        {
            // Correct number of arguments specified, continue.
            boost::filesystem::path directory_value;
            
            // Get the actual directory selected.
            option_directory = vm["dir"].as<std::string>();
        
            // Fetch the correct directory_value based on the specified option.
            if(option_directory == "cli")
            {
                directory_value = inglenook::directories::cli(option_verbose);
            }
            else if(option_directory == "config")
            {
                directory_value = inglenook::directories::config(option_verbose);
            }
            else if(option_directory == "data")
            {
                directory_value = inglenook::directories::data(option_verbose);
            }
            else if(option_directory == "lib")
            {
                directory_value = inglenook::directories::lib(option_verbose);
            }
            else if(option_directory == "log")
            {
                directory_value = inglenook::directories::log(option_verbose);
            }
            else if(option_directory == "sbin")
            {
                directory_value = inglenook::directories::sbin(option_verbose);
            }
            else if(option_directory == "man")
            {
                directory_value = inglenook::directories::man(option_verbose);
            }
            else if(option_directory == "tmp")
            {
                directory_value = inglenook::directories::tmp(option_verbose);
            }
            else if(option_directory == "user")
            {
                directory_value = inglenook::directories::user(option_verbose);
            }
            
            // Did we find a valid option?
            if(!directory_value.empty())
            {
                // See if we need to switch directory.
                if(option_switch)
                {
                    // Switch directory.
                    /// @todo discuss as we cannot change the calling shell's directory, only our own processes directory.
                    chdir(directory_value.string().c_str());
                    
                    // Sucess!
                    success = EXIT_SUCCESS;
                }
                else
                {
                    // Sucess!
                    success = EXIT_SUCCESS;
                }
                
                // Return the directory_value.
                std::cout << directory_value.string() << std::endl;
            }
            else
            {
                // Invalid option!
                std::cerr << "Unrecognised directory option: '" << option_directory << "'" << std::endl;
            }
        }
        else
        {
            // Invalid option!
            std::cerr << "No directory option specified" << std::endl;
        }
    }
    
    // Return whether we was successful.
    return success;
}
