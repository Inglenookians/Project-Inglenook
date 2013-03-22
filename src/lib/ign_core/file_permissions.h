#pragma once

/*
 * file_permissions.h: Class and supporting data types that act as descriptors for basic permissions allocated to a file.
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

// standard library includes
#include <sys/stat.h>

namespace inglenook
{
    namespace core
    {  
        namespace filesystem
        {
         
			/**
			 * file permission types
			 * encapsulates #defines from <sys/stat.h>, it keeps all the relevant options together 
        	 * for intellisense and allows us to create defined standard permission layouts, 
        	 * it may also help with future platforms which don't use *nix file permissions.
			 */
        	enum permission : unsigned int 
        	{
        		
        		// owner can read the file
        		owner_read 		= S_IRUSR,
        				
        		// owner can write to the file
				owner_write 	= S_IWUSR,
				
				// owner can execute contents of the file
				owner_execute	= S_IXUSR,
				
				// group can read the file
				group_read 		= S_IRGRP,
				
				// group can write to the file
				group_write 	= S_IWGRP,
				
				// group can execute contents of the file
				group_execute	= S_IXGRP,
				
				// anyone can read the file
				other_read 		= S_IROTH,
				
				// anyone can write to the file
				other_write 	= S_IWOTH,
				
				// anyone can execute contents of the file
				other_execute	= S_IXOTH,
				
				/// limited permissions (files that should only be used by the writer of the data).
				limited_permissions     = owner_read | owner_write,
				
				/// default permission set applied to files creates which do not specify explicit permissions.
				default_permissions     = owner_read | owner_write | group_read,
				
				/// file that is accessible to all.
				public_permissions      = owner_read | owner_write |group_read | other_read, 
				
				// grant all permissions (this is most likely dangerous).
				dangerous_permissions   = owner_read | owner_write | owner_execute |
				                          group_read | group_write | group_execute |
									  	  other_read | other_write | other_execute
        	};
        
        	/**
        	 * file permissions descriptor object
        	 * describes the permissions of a file.
        	 */
        	class file_permissions
        	{
        		
        	public:
        		
        		/// create a new file permissions object with default permissions
        		file_permissions();
        		
        		/// create a new file permission object with explicit permissions
        		file_permissions(unsigned int permissions);
        		
        		
        		
        		/// test if the permissions described by this object match a given query
        		bool has_permission(const permission& query) const;
        		
        		/// ensure that the permissions described by this object include at least the specified mask
        		void set_permission(const permission& add);
        		
        		/// remove all the permissions in the query from the permissions descriptor.
        		void unset_permission(const permission& remove);
        		

        		
        		/// implicit conversion from unsigned int
        		operator unsigned int () const;
        		
        		
        	private:
        		
        		/// permissions described by this object
        		unsigned int m_permission_bitflags = permission::default_permissions;
        		
        	};
   
        }
    }
}
