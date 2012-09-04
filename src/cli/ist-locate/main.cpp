// STL includes.
#include <iostream>

// Local includes.
#include <ist/Directories.h>

int main(int argc, char* argv[])
{
    // Keep track of our success.
    int success = EXIT_FAILURE;
    
    /// @todo implement proper option handling.
    
    // Check we have the right number of arguments.
    if(argc != 2)
    {
        // Incorrect number!
        std::cerr << "You must specify an option" << std::endl;
    }
    else
    {
        // Correct number of arguments.
        std::string value;
        
        // Retrieve the option chosen.
        std::string option = argv[1];
        
        // Fetch the correct value based on the specified option.
        if(option == "cli")
        {
            value = inglenook::system::directories::cli();
        }
        else if(option == "config_user")
        {
            value = inglenook::system::directories::config_user();
        }
        else if(option == "config_global")
        {
            value = inglenook::system::directories::config_global();
        }
        else if(option == "data")
        {
            value = inglenook::system::directories::data();
        }
        else if(option == "lib")
        {
            value = inglenook::system::directories::lib();
        }
        else if(option == "log")
        {
            value = inglenook::system::directories::log();
        }
        else if(option == "sbin")
        {
            value = inglenook::system::directories::sbin();
        }
        else if(option == "man")
        {
            value = inglenook::system::directories::man();
        }
        else if(option == "tmp")
        {
            value = inglenook::system::directories::tmp();
        }
        
        // Did we find a valid option?
        if(value.empty())
        {
            // Invalid option!
            std::cerr << "Unrecognised option: '" << option << "'" << std::endl;
        }
        else
        {
            // Sucess!
            success = EXIT_SUCCESS;
            
            // Return the value.
            std::cout << value << std::endl;
        }
    }
    
    // Return whether we was successful.
    return success;
}
