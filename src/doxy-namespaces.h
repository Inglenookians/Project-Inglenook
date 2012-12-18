/*
 * doxy-namespaces.h: This file exists as a place to document namespaces for doxygen. 
 *   It is never compiled, and is for reference only.
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


/// Root namespace for all inglenook code.
namespace inglenook
{

	/// Contains code that is core to most, if not all, inglenook applications.
	namespace core 
	{
	
		/// Various exception definitiions (and related elements) used by many inglenook applications.
		namespace exceptions {}
	
	}
	
	/// Contains code related to the various logging tools and libraries within inglenook.
	namespace logging {}

}