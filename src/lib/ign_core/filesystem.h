#pragma once

/*
 * filesystem.h: Methods that support interacton with the filesystem.
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
 * GNU General Public License for more details.l
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// standard library includes
#include <fstream>

// boost (http://boost.org) includes 
#include <boost/filesystem.hpp>

// inglenook includes
#include "file_permissions.h"

namespace inglenook
{
    namespace core
    {  
        namespace filesystem
        {

        	/// opens a file, setting specific permissions. Creates file if does not exist.
        	void create_file(const boost::filesystem::path& file_path, const file_permissions& permissions);
   
        }
    }
}
