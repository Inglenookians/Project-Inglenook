#include "Environment.h"

// STL includes.
#include <iostream>
#include <cerrno>

using namespace inglenook::system;

/// @todo replace string with boost::optional! to deal with blank vs unset variables.
//--------------------------------------------------------//
std::string environment::getVariable(const std::string& variable, const std::string& defaultValue)
{
    // Set the default return value.
    std::string returnValue = defaultValue;
    
    // Try to fetch the environment variable.
    const char* rawValue = std::getenv(variable.c_str());
    
    // Check whether we was successful.
    if(rawValue != NULL)
    {
        // Sucessful fetch, store the value to be returned.
        returnValue = rawValue;
    }
    
    // Return the value.
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
bool environment::setVariable(const std::string& variable, const std::string& value, bool overwrite)
{
    bool success = false;
    
    // Try to set the environment variable.
    if(setenv(variable.c_str(), value.c_str(), overwrite) == 0)
    {
        // Success!
        success = true;
    }
    else
    {
        // Error trying to set environment variable
        std::cerr << "Error trying to set environment variable '" << variable << "' to '" << value << "': " << std::strerror(errno) << std::endl;
    }
    
    // Return our sucess.
    return success;
}
//--------------------------------------------------------//
