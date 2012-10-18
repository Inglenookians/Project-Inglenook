#pragma once

/*
 * directories.h: Retrieve common directory paths.
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

// boost (http://boost.org) includes
#include <boost/filesystem.hpp>

namespace inglenook
{
    namespace directories
    {
        
        /**
         * Fetch cli directory.
         * @param explain Whether to print to std::cout how the directory was selected.
         * @return the location of the cli directory.
         */
        const boost::filesystem::path cli(bool explain = false);
            
        /**
         * Fetch config directory.
         * @param explain Whether to print to std::cout how the directory was selected.
         * @return the location of the config directory.
         */
        const boost::filesystem::path config(bool explain = false);
            
        /**
         * Fetch data directory.
         * @param explain Whether to print to std::cout how the directory was selected.
         * @return the location of the data directory.
         */
        const boost::filesystem::path data(bool explain = false);
            
        /**
         * Fetch lib directory.
         * @param explain Whether to print to std::cout how the directory was selected.
         * @return the location of the lib directory.
         */
        const boost::filesystem::path lib(bool explain = false);
            
        /**
         * Fetch log directory.
         * @param explain Whether to print to std::cout how the directory was selected.
         * @return the location of the log directory.
         */
        const boost::filesystem::path log(bool explain = false);
            
        /**
         * Fetch sbin directory.
         * @param explain Whether to print to std::cout how the directory was selected.
         * @return the location of the sbin directory.
         */
        const boost::filesystem::path sbin(bool explain = false);
            
        /**
         * Fetch man directory.
         * @param explain Whether to print to std::cout how the directory was selected.
         * @return the location of the man directory.
         */
        const boost::filesystem::path man(bool explain = false);
            
        /**
         * Fetch tmp directory.
         * @param explain Whether to print to std::cout how the directory was selected.
         * @return the location of the tmp directory.
         */
        const boost::filesystem::path tmp(bool explain = false);
        
        /**
         * Fetch local user directory.
         * @param explain Whether to print to std::cout how the directory was selected.
         * @return the location of the local user directory.
         */
        const boost::filesystem::path user(bool explain = false);
        
    }
}
