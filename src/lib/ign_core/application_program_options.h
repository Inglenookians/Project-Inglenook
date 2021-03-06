/*
 * application_program_options.h: Defines the standard application program options.
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

// Program options: help
const std::string PO_HELP_FULL = "help";

// Program options: version
const std::string PO_VERSION_FULL = "version";

// Program options: config file
const std::string PO_CONFIG_FILE_FULL = "config-file";

// Program options: config command line prefix
const std::string PO_CONFIG_COMMAND_PREFIX = "-conf-";

// Program options: ignore config command line
const std::string PO_IGNORE_CONFIG_COMMAND = "ignore-config-command";
