#pragma once
/*
 * log_entry_modifiers.h: Simple stream modifiers for changing the log entry.
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

namespace logging
{

/**
 * Modifies the logs name space
 */
class ns
{

    public:

        /**
         * Creates a new name space stream modifier
         * @param value new name space
         */
        ns(const std::string& value)
        {
            m_ns = value;
        }

        /**
         * log namespace to apply
         * @return log namespace
         */
        const std::string& log_namespace() const
        {
            return m_ns;
        }

    private:
    
        /// the name space to apply
        std::string m_ns;
    
};

/**
 * Adds extended data to the log.
 */
class log_data
{

    public:

        /**
         * Adds a data item to the log.
         * @param key key for the data type.
         * @param value new name space
         */
        log_data(const std::string& key, const std::string& value)
        {
            m_value = value;
            m_key = key;
        }

        /**
         * Key of data pair
         * @return Data pairs key
         */
        const std::string& key() const
        {
            return m_key;
        }

        /**
         * Value of data pair
         * @return Data pairs value
         */
        const std::string& value() const
        {
            return m_value;
        }


    private:

        /// the key of the data
        std::string m_key;

        /// the value of the data
        std::string m_value;

};

} // namespace inglenook::logging

} // namespace inglenook
