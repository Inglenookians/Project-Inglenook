/*
* all_tests.h: Runs all logging tests to make sure the logging mechanism is working
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
// name to apply to all tests
#define LOG_TEST_NAME ign_logging_tests

//
// This is the manifest for logging tests, please note that the following
// elements are currently outside of test coverage:
//		log_exceptions.h 	- defines logging exception structures, no real functional code.
//		logging.h			- include file for dependants to initialize logging easily.
//		all examples		- these items are not tested and are not considered *functional* code.
//

#include "log_entry_tests.h"
#include "log_entry_buffered_tests.h"
#include "log_entry_modifiers_tests.h"
#include "log_writer_tests.h"
#include "log_client_tests.h"
