#include "Directories.h"

// Local includes.
#include "Environment.h"
#include "Config.h"

using namespace inglenook::system;

//--------------------------------------------------------//
std::string directories::cli()
{
    // Check environment variable
    std::string returnValue = environment::getVariable(environment::ENV_DIR_CLI);
    if(returnValue.empty())
    {
        // Check user configuration
        returnValue = config::getUser("inglenook.system.directories.cli");
        if(returnValue.empty())
        {
            // Check global configuration
            returnValue = config::getGlobal("inglenook.system.directories.cli");
            if(returnValue.empty())
            {
                // Return default.
                returnValue = directories::DEFAULT_DIR_CLI;
            }
        }
    }
    
    // Return the value.
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string directories::config_global()
{
    // Check environment variable
    std::string returnValue = environment::getVariable(environment::ENV_DIR_CONFIG_GLOBAL);
    if(returnValue.empty())
    {
        // We cannot check user or global configurations as this could cause a cyclic-dependency on each other.
        /// @todo discuss this with team for correct handling of this issue.
        
        // We therefore return the default.
        returnValue = directories::DEFAULT_DIR_CONFIG_GLOBAL;
    }
    
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string directories::config_user()
{
    // Check environment variable
    std::string returnValue = environment::getVariable(environment::ENV_DIR_CONFIG_USER);
    if(returnValue.empty())
    {
        // We cannot check user or global configurations as this could cause a cyclic-dependency on each other.
        /// @todo discuss this with team for correct handling of this issue.
        
        // We therefore return the default.
        returnValue = directories::DEFAULT_DIR_CONFIG_USER;
    }
    
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string directories::data()
{
    // Check environment variable
    std::string returnValue = environment::getVariable(environment::ENV_DIR_DATA);
    if(returnValue.empty())
    {
        // Check user configuration
        returnValue = config::getUser("inglenook.system.directories.data");
        if(returnValue.empty())
        {
            // Check global configuration
            returnValue = config::getGlobal("inglenook.system.directories.data");
            if(returnValue.empty())
            {
                // Return default.
                returnValue = directories::DEFAULT_DIR_DATA;
            }
        }
    }
    
    // Return the value.
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string directories::lib()
{
    // Check environment variable
    std::string returnValue = environment::getVariable(environment::ENV_DIR_LIB);
    if(returnValue.empty())
    {
        // Check user configuration
        returnValue = config::getUser("inglenook.system.directories.lib");
        if(returnValue.empty())
        {
            // Check global configuration
            returnValue = config::getGlobal("inglenook.system.directories.lib");
            if(returnValue.empty())
            {
                // Return default.
                returnValue = directories::DEFAULT_DIR_LIB;
            }
        }
    }
    
    // Return the value.
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string directories::log()
{
    // Check environment variable
    std::string returnValue = environment::getVariable(environment::ENV_DIR_LOG);
    if(returnValue.empty())
    {
        // Check user configuration
        returnValue = config::getUser("inglenook.system.directories.log");
        if(returnValue.empty())
        {
            // Check global configuration
            returnValue = config::getGlobal("inglenook.system.directories.log");
            if(returnValue.empty())
            {
                // Return default.
                returnValue = directories::DEFAULT_DIR_LOG;
            }
        }
    }
    
    // Return the value.
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string directories::sbin()
{
    // Check environment variable
    std::string returnValue = environment::getVariable(environment::ENV_DIR_SBIN);
    if(returnValue.empty())
    {
        // Check user configuration
        returnValue = config::getUser("inglenook.system.directories.sbin");
        if(returnValue.empty())
        {
            // Check global configuration
            returnValue = config::getGlobal("inglenook.system.directories.sbin");
            if(returnValue.empty())
            {
                // Return default.
                returnValue = directories::DEFAULT_DIR_SBIN;
            }
        }
    }
    
    // Return the value.
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string directories::man()
{
    // Check environment variable
    std::string returnValue = environment::getVariable(environment::ENV_DIR_MAN);
    if(returnValue.empty())
    {
        // Check user configuration
        returnValue = config::getUser("inglenook.system.directories.man");
        if(returnValue.empty())
        {
            // Check global configuration
            returnValue = config::getGlobal("inglenook.system.directories.man");
            if(returnValue.empty())
            {
                // Return default.
                returnValue = directories::DEFAULT_DIR_MAN;
            }
        }
    }
    
    // Return the value.
    return returnValue;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
std::string directories::tmp()
{
    // Check environment variable
    std::string returnValue = environment::getVariable(environment::ENV_DIR_TMP);
    if(returnValue.empty())
    {
        // Check user configuration
        returnValue = config::getUser("inglenook.system.directories.tmp");
        if(returnValue.empty())
        {
            // Check global configuration
            returnValue = config::getGlobal("inglenook.system.directories.tmp");
            if(returnValue.empty())
            {
                // Return default.
                returnValue = directories::DEFAULT_DIR_TMP;
            }
        }
    }
    
    // Return the value.
    return returnValue;
}
//--------------------------------------------------------//
