/*
 * log_entry_buffered.h: Represents a single, buffered, log entry.
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

// standard library includes
#include <sstream>

// inglenook includes
#include "log_entry.h"

namespace inglenook
{

namespace logging
{

/**
 * log_entry_buffered
 * Implementation of log_entry which uses a string stream to back the message property.
 * This gives much greater flexibility, at the cost of a little overhead. Designed for use with writers.
 */
class log_entry_buffered: public log_entry
{

	public:

		/// creates a new buffered log entry.
		log_entry_buffered();

		/// deconstructs an buffered log entry.
		virtual ~log_entry_buffered();

		/// sets the log message
		std::stringstream& message_buffer();

		/// virtual member of log_entry
		virtual const std::string& message() override;

		/// sets the log message
		virtual void message(const std::string& value) override;

	private:

		/// log message content
		std::stringstream m_message_buffer;

};

} // namespace inglenook::logging

} // namespace inglenook
