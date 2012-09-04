#include <string>

namespace inglenook
{
    namespace system
    {  
        namespace environment
        {          
            // Define environment variables.
            // Directories
            const std::string ENV_DIR_CLI =           "INGLENOOK_DIR_CLI";
            const std::string ENV_DIR_CONFIG_GLOBAL = "INGLENOOK_DIR_CONFIG_GLOBAL";
            const std::string ENV_DIR_CONFIG_USER =   "INGLENOOK_DIR_CONFIG_USER";
            const std::string ENV_DIR_DATA =          "INGLENOOK_DIR_DATA";
            const std::string ENV_DIR_LIB =           "INGLENOOK_DIR_LIB";
            const std::string ENV_DIR_LOG =           "INGLENOOK_DIR_LOG";
            const std::string ENV_DIR_SBIN =          "INGLENOOK_DIR_SBIN";
            const std::string ENV_DIR_MAN =           "INGLENOOK_DIR_MAN";
            const std::string ENV_DIR_TMP =           "INGLENOOK_DIR_TMP";
            
            /**
             * Get an environment variable.
             * @param variable The name of the environment variable.
             * @param defaultValue The default value to return if no value set.
             * @return The value to of the environment variable.
             */
            std::string getVariable(const std::string& variable, const std::string& defaultValue = "");
            
            /**
             * Set an environment variable.
             * @param variable The name of the environment variable.
             * @param value The value to set the environment variable to.
             * @param overwrite Whether to overwrite the value if it already is set.
             * @note Setting an environment variable will affect the runtime of this process only.
             */
            bool setVariable(const std::string& variable, const std::string& value, bool overwrite = true);
            
        };
    }
}
