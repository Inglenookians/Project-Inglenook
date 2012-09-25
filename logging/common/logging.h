/*
* logging.h: External interface for client applications. Not use in common logging directly.
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
#ifndef LOGGING_H_
#define LOGGING_H_

#include "log_client.h"

namespace inglenook
{

	/// create the log writer (used by most applications).
	static auto log_output = logging::log_writer::create();

	/// thread safe logging interface.
	static logging::log_client ilog(log_output);


}

#endif /* LOGGING_H_ */
