/*
 * file_permissions.cpp: Class and supporting data types that act as descriptors for basic permissions allocated to a file.
 * Copyright (C) 2013, Project Inglenook (http://www.project-inglenook.co.uk)
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

#include "file_permissions.h"

using namespace inglenook::core::filesystem;


/**
 * Creates a new file permissions descriptor
 */
file_permissions::file_permissions() 
	: file_permissions(permission::default_permissions)
{
	// defers to other constructor
}

/**
 * Creates a new file permissions descriptor
 * @param permissions permissions to instance descriptor with.
 */
file_permissions::file_permissions(unsigned int permissions)
{
	m_permission_bitflags = permissions;	
}

/**
 * Checks if the descriptor has specific options set.
 * @param query item being tested for.
 * @returns true if query is fully matched
 */
bool file_permissions::has_permission(const permission& query) const
{
	return (m_permission_bitflags & query) == query;
}

/**
 * Sets specific options on the permissions
 * @param add permissions to add to current descriptor
 */
void file_permissions::set_permission(const permission& add)
{
	m_permission_bitflags |= add;
}

/**
 * Unsets specific options on the permissions
 * @param remove permissions to remove from current descriptor
 */
void file_permissions::unset_permission(const permission& remove)
{
	m_permission_bitflags &= ~(remove);
}

/**
 * implicit conversion for the descriptor back to a unsignd int (or mode_t).
 */
file_permissions::operator unsigned int() const
{
	// return permissions
	return m_permission_bitflags;
}