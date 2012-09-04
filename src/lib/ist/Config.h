#include <string>

namespace inglenook
{
    namespace system
    {  
        namespace config
        {
            
            /**
             * Get a user config value.
             * @param key The key of the user config value to fetch.
             * @param defaultValue The default value to return if no value set.
             * @return The value for the key in the user config.
             */
            std::string getUser(const std::string& key, const std::string& defaultValue = "");
            
            /**
             * Set a user config value.
             * @param variable The key of the user config value.
             * @param value The value to set for the key.
             * @param overwrite Whether to overwrite the value if it already is set.
             */
            bool setUser(const std::string& key, const std::string& value, bool overwrite = true);
            
            /**
             * Get a global config value.
             * @param key The key of the global config value to fetch.
             * @param defaultValue The default value to return if no value set.
             * @return The value for the key in the global config.
             */
            std::string getGlobal(const std::string& key, const std::string& defaultValue = "");
            
            /**
             * Set a global config value.
             * @param variable The key of the global config value.
             * @param value The value to set for the key.
             * @param overwrite Whether to overwrite the value if it already is set.
             */
            bool setGlobal(const std::string& key, const std::string& value, bool overwrite = true);
            
            /**
             * Get a config value from a file.
             * @param key The key of the config value to fetch.
             * @param defaultValue The default value to return if no value set.
             * @return The value for the key in the config file.
             */
            std::string get(const std::string& filePath, const std::string& key, const std::string& defaultValue = "");
            
            /**
             * Set a config value to a file.
             * @param variable The key of the global config value.
             * @param value The value to set for the key.
             * @param overwrite Whether to overwrite the value if it already is set.
             */
            bool set(const std::string& filePath, const std::string& key, const std::string& value, bool overwrite = true);
            
        };
    }
}
