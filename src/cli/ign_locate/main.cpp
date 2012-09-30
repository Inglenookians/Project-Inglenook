/*
 * main.cpp: @todo fill out!
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
#include <ign_directories/directories.h>

// standard library includes
#include <iostream>

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

int main(int argc, char* argv[])
{
    // Keep track of our success.
    int success = EXIT_FAILURE;
    
    // Keep track of user switches.
    bool switch_directory = false;
    bool verbose_output = false;
    
    /*
    /// @todo implement proper option handling.
    // ign_locate directory [options]
    // -s switch Automatically change directory once the directory is found, equivent of 'cd (ign_locate directory)'.
    // -v verbose Detailed output indicate how it came to its conclusion (detailing where it looked to find the directory).
    
    // Generic options (help and version).
    boost::program_options::options_description generic("Generic options");
    generic.add_options()
        ("version,v", "produce version information")
        ("help,h", "produce help message")
    ;
    
    // Program specific options.
    boost::program_options::options_description config("Configuration");
    config.add_options()
        ("switch,s", "switch description")
        ("verbose,V", "verbose description")
    ;
    
    // Set the positional options.
    boost::program_options::positional_options_description p;
    p.add("input-file", -1);
    
    // Setup the program options (Joins options together).
    boost::program_options::options_description cmdline_options;
    cmdline_options.add(generic).add(config);
    
    // Parse the arguments!
    boost::program_options::variables_map vm;
    //boost::program_options::store(boost::program_options::parse_command_line(argc, argv, cmdline_options), vm);
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
    boost::program_options::notify(vm);
    
    if (vm.count("help"))
    {
        std::cout << cmdline_options;
    }
    */
    
    // Check we have the right number of arguments.
    if(argc != 2)
    {
        // Incorrect number!
        std::cerr << "You must specify an option" << std::endl;
    }
    else
    {
        // Correct number of arguments.
        boost::filesystem::path value;
        
        // Retrieve the option chosen.
        std::string option = argv[1];
        
        // Fetch the correct value based on the specified option.
        if(option == "cli")
        {
            value = inglenook::directories::cli(true);
        }
        else if(option == "config")
        {
            value = inglenook::directories::config();
        }
        else if(option == "data")
        {
            value = inglenook::directories::data();
        }
        else if(option == "lib")
        {
            value = inglenook::directories::lib();
        }
        else if(option == "log")
        {
            value = inglenook::directories::log();
        }
        else if(option == "sbin")
        {
            value = inglenook::directories::sbin();
        }
        else if(option == "man")
        {
            value = inglenook::directories::man();
        }
        else if(option == "tmp")
        {
            value = inglenook::directories::tmp();
        }
        else if(option == "user")
        {
            value = inglenook::directories::user();
        }
        
        // Did we find a valid option?
        if(value.empty())
        {
            // Invalid option!
            std::cerr << "Unrecognised option: '" << option << "'" << std::endl;
        }
        else
        {
            // See if we need to switch directory.
            if(switch_directory)
            {
                // Switch directory.
                /// @todo implement!
                chdir(value.string().c_str());
                
                // Sucess!
                success = EXIT_SUCCESS;
            }
            else
            {
                // Sucess!
                success = EXIT_SUCCESS;
            }
            
            // Return the value.
            std::cout << value.string() << std::endl;
        }
    }
    
    // Return whether we was successful.
    return success;
}
