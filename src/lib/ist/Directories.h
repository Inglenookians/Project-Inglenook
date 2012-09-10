#include <string>

namespace inglenook
{
    namespace system
    {  
        namespace directories
        {
            // Define default directories.
            const std::string DEFAULT_DIR_CLI =           "/usr/local/bin";
            const std::string DEFAULT_DIR_CONFIG_GLOBAL = "/etc/inglenook";
            const std::string DEFAULT_DIR_CONFIG_USER =   "~/.inglenook";
            const std::string DEFAULT_DIR_DATA =          "/var/lib/inglenook";
            const std::string DEFAULT_DIR_LIB =           "/usr/local/lib";
            const std::string DEFAULT_DIR_LOG =           "/var/log/inglenook";
            const std::string DEFAULT_DIR_SBIN =          "/usr/local/sbin";
            const std::string DEFAULT_DIR_MAN =           "/usr/local/share/man";
            const std::string DEFAULT_DIR_TMP =           "/var/tmp/inglenook";
            
            /**
             * Fetch cli directory.
             * @return the location of the cli directory.
             */
            std::string cli();
            
            /**
             * Fetch global config directory.
             * @return the location of the global config directory.
             */
            std::string config_global();
            
            /**
             * Fetch user config directory.
             * @return the location of the user config directory.
             */
            std::string config_user();
            
            /**
             * Fetch data directory.
             * @return the location of the data directory.
             */
            std::string data();
            
            /**
             * Fetch lib directory.
             * @return the location of the lib directory.
             */
            std::string lib();
            
            /**
             * Fetch log directory.
             * @return the location of the log directory.
             */
            std::string log();
            
            /**
             * Fetch sbin directory.
             * @return the location of the sbin directory.
             */
            std::string sbin();
            
            /**
             * Fetch man directory.
             * @return the location of the man directory.
             */
            std::string man();
            
            /**
             * Fetch tmp directory.
             * @return the location of the tmp directory.
             */
            std::string tmp();
            
        };
    }
}
