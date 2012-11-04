#
# CMakeLists.txt: CMake configuration file.
# Copyright (C) 2012, Project Inglenook (http://www.project-inglenook.co.uk)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

###################
# Logging Library #
###################

# Make the library.
add_library(
	ign_logging 
	SHARED
	log_client.cpp
	log_entry_buffered.cpp
	log_entry.cpp
	log_writer.cpp
)

# Link to required libraries
target_link_libraries(
	ign_logging
	boost_thread
	boost_filesystem
	ign_core
	ign_directories
)

# Set the properties
set_target_properties(
	ign_logging PROPERTIES
	VERSION 0.0.1
	SOVERSION 1
)

# Specify the install location.
install(
	TARGETS ign_logging 
	DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

#########
# Tests #
#########

# Make the test.
add_executable(
	ign_logging_tests 
	all_tests.cpp
)

# Link to required libraries
target_link_libraries(
	ign_logging_tests
	ign_logging
	boost_unit_test_framework
	boost_regex
) 

# Add the test.
add_test(
	NAME ign_logging_tests
	COMMAND ign_logging_tests
)
