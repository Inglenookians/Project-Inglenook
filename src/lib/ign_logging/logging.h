/*
* logging.h: External interface for client applications. Full implementation in common logging.
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
#pragma once

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>

// inglenook includes
#include "log_writer.h"
#include "log_client.h"
#include "log_entry_modifiers.h"

// the following precompiler is designed such that the SHARED definition
// declarations the appropriate export or import keywords relative the
// compilation context (ign_logging) will export, others will import.
// it also defines INITIALIZE_TO which is designed to allow ign_logging
// to initialize global variables to known values but is ignored by "import-ers".
#ifndef IGN_LOGGING_LIBRARY
	#ifdef _WIN32
		#warning "WIN32 is not supported. Preparing the compiler to cry."
		#define SHARED extern "C++" __declspec(dllimport)
	#else
		#define SHARED extern "C++"
	#endif
	#define INITIALIZE_TO(x) /* do nothing */
#else
	#ifdef _WIN32
		#warning "WIN32 is not supported. Preparing the compiler to cry."
		#define SHARED __declspec(dllexport)
	#else
		#define SHARED
	#endif
	#define INITIALIZE_TO(x) =(x)
#endif

namespace inglenook
{

namespace logging
{

	// initializes the logging system
	SHARED void initialize_logging();

	// initializes the logging system with a define file path
	SHARED void initialize_logging(const boost::filesystem::path& log_file);

	/// log output interface - default serialization object.
	SHARED std::shared_ptr<log_writer> log_output INITIALIZE_TO(nullptr);

	/// log client interface - provides stream access to log writing.
	SHARED std::shared_ptr<log_client> ilog INITIALIZE_TO(nullptr);

} // namespace logging

} // namespace inglenook
