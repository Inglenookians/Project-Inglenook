#pragma once

/*
 * environment.h: Store and retrieve environment variable values.
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

// standard library includes
#include <string>

namespace inglenook
{
    namespace core
    {  
        namespace environment
        {
            
            /**
             * Get an environment variable.
             * @param variable The name of the environment variable.
             * @param default_value The default value to return if no value is set.
             * @return The value of the environment variable.
             */
            std::string get(const std::string& variable, const std::string& default_value = "");
            
            /**
             * Set an environment variable.
             * @param variable The name of the environment variable.
             * @param value The value to set the environment variable to.
             * @param overwrite Whether to overwrite the value if it is already set.
             * @note Setting an environment variable will only affect the runtime of this process.
             */
            void set(const std::string& variable, const std::string& value, bool overwrite = true);
            
        }
    }
}
