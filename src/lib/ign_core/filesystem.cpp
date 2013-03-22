/*
 * filesystem.cpp: Methods that support interacton with the filesystem.
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
#include <sys/stat.h>
#include <fstream>

// boost (http://boost.org) includes
#include <boost/scope_exit.hpp>

// inglenook includes
#include "filesystem.h"

using namespace inglenook::core;


/**
 * Creates an output stream for the specified file, creating the file if it does not exist.
 * @param file_path path to the file to open.
 * @param permissions permissions to assign if the file needs to be created.
 */
void filesystem::create_file(const boost::filesystem::path& file_path, const filesystem::file_permissions& permissions)
{
	 	 
	// using c api's, to const char * as go.
	auto file_path_as_cstr = file_path.native().c_str();
	
	// create file, set permissions
	std::ofstream file(file_path_as_cstr);
	chmod(file_path_as_cstr, permissions);
	
	// no support just yet for gcc - monitor;
	// http://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html
	// ref: 27.9 file-based streams (partial) - missing move and swap operations.
	
	// TODO: change this method to return the file stream once libstdc++ supports move semantics on std::ofstream (url/ref in comment above).
	// yay c++11 move semantics! 
	//return file;
	
}