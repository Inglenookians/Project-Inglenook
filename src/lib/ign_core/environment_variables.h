#pragma once

/*
 * environment_variables.h: Defines commonly used environment variables.
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
            namespace variables
            {
                
                // Define environment variables.
                // Directories
                const std::string DIR_CLI =     "INGLENOOK_DIR_CLI";
                const std::string DIR_CONFIG =  "INGLENOOK_DIR_CONFIG";
                const std::string DIR_DATA =    "INGLENOOK_DIR_DATA";
                const std::string DIR_LIB =     "INGLENOOK_DIR_LIB";
                const std::string DIR_LOG =     "INGLENOOK_DIR_LOG";
                const std::string DIR_SBIN =    "INGLENOOK_DIR_SBIN";
                const std::string DIR_MAN =     "INGLENOOK_DIR_MAN";
                const std::string DIR_TMP =     "INGLENOOK_DIR_TMP";
                const std::string DIR_USER =    "INGLENOOK_DIR_USER";
                
            }
        }
    }
}
