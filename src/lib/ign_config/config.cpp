/*
 * config.cpp: Store and retrieve configuration values from command line, application and global config files.
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

#include "config.h"

// inglenook includes
#include "config_exceptions.h"
#include <ign_core/application.h>
#include <ign_directories/directories.h>

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/locale.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/optional.hpp>

using namespace inglenook;

//--------------------------------------------------------//
namespace
{
    /// Cache of the command line configuration settings.
    boost::optional<boost::property_tree::ptree> m_command_line_config;
    /// Cache of the application configuration settings.
    boost::optional<boost::property_tree::ptree> m_application_config;
    /// Cache of the global configuration settings.
    boost::optional<boost::property_tree::ptree> m_global_config;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
boost::optional<std::string> config::get(const std::string& key, bool skip_blank, const boost::optional<std::string>& default_value)
{
    // First check the command line config file.
    // We do not pass the default value, as we want to know specifically if it has been set.
    auto return_value(command_line::get(key));
    
    // Did we not get a value or have we specified to skip blank values.
    if(!return_value || (skip_blank && (*return_value).empty()))
    {   
        // Get it from the application config instead.
        // We do not pass the default value, as we want to know specifically if it has been set.
        return_value = app::get(key);
        
        // Did we not get a value or have we specified to skip blank values.
        if(!return_value || (skip_blank && (*return_value).empty()))
        {
            // Get it from the global config instead.
            return_value = global::get(key, default_value);
        }
    }
    
    // Return the value.
    return return_value;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
boost::optional<std::string> config::command_line::get(const std::string& key, const boost::optional<std::string>& default_value)
{
    // Set the default return config value.
    auto return_value(default_value);
    
    // Check whether it exists in the application's config arguments first.
    auto itrFind = core::application::config_arguments().find(key);
    if(itrFind != core::application::config_arguments().end())
    {
        // Fetch the key value from the application's config arguments.
        return_value = itrFind->second;
    }
    else
    {
        // Has the config file been loaded?
        if(!m_command_line_config)
        {
            // Load the file into cache.
            m_command_line_config = cache::load(core::application::config_file(), false);
        }
        
        // Fetch the key value from the application's config file.
        return_value = cache::get((*m_command_line_config), key, default_value);
    }
    
    // Return the key value.
    return return_value;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
boost::filesystem::path config::app::filepath()
{
    // Return the application config file.
    return boost::filesystem::path(directories::config() / (core::application::name() +  "_config.xml"));
}
//--------------------------------------------------------//

//--------------------------------------------------------//
boost::optional<std::string> config::app::get(const std::string& key, const boost::optional<std::string>& default_value)
{
    // Has the file been loaded?
    if(!m_application_config)
    {
        // Load the file into cache.
        m_application_config = cache::load(app::filepath(), false);
    }
    
    // Fetch and return the key value from the application config.
    return cache::get((*m_application_config), key, default_value);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
boost::filesystem::path config::global::filepath()
{
    // Return the global config file.
    return boost::filesystem::path(directories::config() / "config.xml");
}
//--------------------------------------------------------//

//--------------------------------------------------------//
boost::optional<std::string> config::global::get(const std::string& key, const boost::optional<std::string>& default_value)
{
    // Has the file been loaded?
    if(!m_global_config)
    {
        // Load the file into cache.
        m_global_config = cache::load(global::filepath());
    }
    
    // Fetch and return the key value from the global config.
    return cache::get((*m_global_config), key, default_value);
}
//--------------------------------------------------------//

//--------------------------------------------------------//
boost::property_tree::ptree config::cache::load(const boost::filesystem::path& file_path, const bool verbose_file_warning)
{
    // Set the default return configuration settings.
    boost::property_tree::ptree return_ptree;
    
    // Check whether the file exists.
    if(boost::filesystem::exists(file_path))
    {
        // Get a lock on the file.
        boost::interprocess::file_lock flock(file_path.string().c_str());
        boost::interprocess::scoped_lock<boost::interprocess::file_lock> exclusive_lock(flock);
        
        // Try and read the configuration file and then fetch the value.
        try
        {
            // Read the configuration file.
            boost::property_tree::xml_parser::read_xml(file_path.string(), return_ptree);
        }
        catch(boost::property_tree::xml_parser::xml_parser_error &ex)
        {
            // Error! Failed to read configuration file.
            /// @todo Investigate whether this should raise a log entry.
            std::cerr << boost::format(boost::locale::translate("ERROR: failed to read configuration file: '%1%'")) % ex.what() << std::endl;

            BOOST_THROW_EXCEPTION(exceptions::config_file_read_exception()
                                  << exceptions::config_file(file_path)
                                  );
        }
    }
    else
    {
        // Warning! Configuration file does not exist, be vebose if required.
        if(verbose_file_warning)
        {
            /// @todo Investigate whether this should raise a log entry.
            std::cerr << boost::format(boost::locale::translate("WARNING: configuration file does not exist: '%1%'")) % file_path.string() << std::endl;

            /// @note We choose not to throw an exception as the cache loading does not require the config file to exist.
        }
    }
    
    // Return the configuration settings.
    return return_ptree;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
boost::optional<std::string> config::cache::get(const boost::property_tree::ptree& ptree, const std::string& key, const boost::optional<std::string>& default_value)
{
    // Fetch the value for the key from the cache.
    auto return_value(ptree.get_optional<std::string>(key));
    
    // If we didn't get a value and a default value has been set.
    if(!return_value && default_value)
    {
        // Set the return value to the default value.
        return_value = default_value;
    }
    
    // Return the value.
    return return_value;
}
//--------------------------------------------------------//

//--------------------------------------------------------//
void config::cache::clear()
{
    // Reset all the caches.
    m_command_line_config.reset();
    m_application_config.reset();
    m_global_config.reset();
}
//--------------------------------------------------------//
