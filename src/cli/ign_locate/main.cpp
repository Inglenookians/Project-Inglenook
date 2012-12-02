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
#include "version.h"
#include <ign_core/application.h>
#include <ign_core/application_exceptions.h>
#include <ign_directories/directories.h>

// standard library includes
#include <iostream>

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/locale.hpp>
#include <boost/program_options.hpp>

int main(int argc, char* argv[])
{
    // Keep track of our success.
    int success(EXIT_FAILURE);
    
    // Program description.
    std::string description(boost::locale::translate("a tool for locating various special inglenook directories"));
    
    // Create the application store.
    inglenook::core::application app(description, VERSION,  __DATE__, __TIME__);
    
    // Program specific options.
    boost::program_options::options_description options("Program options");
    options.add_options()
        ("dir,d", boost::program_options::value<std::string>()->required(),
            boost::locale::translate("REQUIRED: The directory to locate. Available options are:\n\n"
                                     "cli     \tThe location of inglenook's command line tools\n"
                                     "config  \tThe location of inglenook's configuration files\n"
                                     "data    \tThe location of inglenook's data files\n"
                                     "lib     \tThe location of inglenook's internal libraries\n"
                                     "log     \tThe location of inglenook's log files\n"
                                     "sbin    \tThe location of inglenook's system administrator tools\n"
                                     "man     \tThe location of inglenook's manual pages\n"
                                     "tmp     \tThe location of inglenook's temporary files\n"
                                     "user    \tThe location of user's home directory\n").str().c_str())
        ("verbose,v", boost::locale::translate("Detailed output indicate how it came to its conclusion (detailing where it looked to find the directory)").str().c_str())
    ;
    
    // Set the positional options.
    boost::program_options::positional_options_description positions;
    positions.add("dir", 1);
    
    // Try and parse the command line arguments.
    bool parser_exit(false);
    bool parser_failure(false);
    boost::program_options::variables_map vm;
    try
    {
        parser_exit = inglenook::core::application::arguments_parser(vm, argc, argv, options, positions);
    }
    catch(inglenook::core::exceptions::application_arguments_parser_exception &ex)
    {
        parser_failure = true;
    }
    
    // Has the options parser indicated we should exit.
    if(parser_exit)
    {
        // Set as a success execution before we finish.
        success = EXIT_SUCCESS;
    }
    // Did the parser fail (It would have already printed out the issue .
    else if(parser_failure)
    {
        // Set as a failure execution before we finish.
        success = EXIT_FAILURE;
    }
    else
    {
        // Did the user request verbose output?
        bool option_verbose(false);
        if(vm.count("verbose"))
        {
            // They did!
            option_verbose = true;
        }
        
        // Get the actual directory selected.
        std::string option_directory(vm["dir"].as<std::string>());
        boost::filesystem::path directory_value;
    
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
        else
        {
            // Invalid option!
            std::cerr << boost::format(boost::locale::translate("ERROR: unrecognised directory option: '%1%'")) % option_directory << std::endl;
        }
        
        // Did we find a valid option?
        if(!directory_value.empty())
        {
            // Return the directory_value.
            std::cout << directory_value.string() << std::endl;
            
            // Sucess!
            success = EXIT_SUCCESS;
        }
        else
        {
            // Invalid: cannot have empty directory option!
            std::cerr << boost::format(boost::locale::translate("ERROR: failed to find directory: '%1%'")) % option_directory << std::endl;
        }
    }
    
    // Return whether we was successful.
    return success;
}
