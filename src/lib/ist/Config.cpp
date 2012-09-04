#include "Config.h"

using namespace inglenook::system;

//--------------------------------------------------------//
std::string config::getUser(const std::string& key, const std::string& defaultValue)
{
    // Find the user config file.
    /// @todo implement.
    std::string filePath = "";
    
    // Fetch and return the key value from the user config file.
    return get(filePath, key, defaultValue);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
bool config::setUser(const std::string& key, const std::string& value, bool overwrite)
{
    // Find the user config file.
    /// @todo implement.
    std::string filePath = "";
    
    // Set the value for the key in the user config file and return success.
    return set(filePath, key, value, overwrite);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string config::getGlobal(const std::string& key, const std::string& defaultValue)
{
    // Find the global config file.
    /// @todo implement.
    std::string filePath = "";
    
    // Fetch and return the key value from the global config file.
    return get(filePath, key, defaultValue);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
bool config::setGlobal(const std::string& key, const std::string& value, bool overwrite)
{
    // Find the global config file.
    /// @todo implement.
    std::string filePath = "";
    
    // Set the value for the key in the global config file and return success.
    return set(filePath, key, value, overwrite);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string config::get(const std::string& filePath, const std::string& key, const std::string& defaultValue)
{
    // Set the default return value.
    std::string returnValue = defaultValue;
    
    /// @todo implement.
    
    // Return the value.
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
bool config::set(const std::string& filePath, const std::string& key, const std::string& value, bool overwrite)
{
    bool success = false;
    
    /// @todo implement.
    
    // Return our sucess.
    return success;
}
//--------------------------------------------------------//
