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

###########
# Library #
###########

# Make the library.
add_library(
	ign_core 
	SHARED
	application.cpp
	environment.cpp
)

# Link to required libraries
target_link_libraries(
	ign_core
	boost_filesystem
	boost_locale
	boost_program_options
	boost_system
)

# Set the properties
set_target_properties(
	ign_core PROPERTIES
	VERSION 1.0.0
	SOVERSION 1
)

# Specify the install location.
install(
	TARGETS ign_core 
	DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

#########
# Tests #
#########

# Make the test.
add_executable(
	ign_core_tests 
	tests.cpp
)

# Link to required libraries
target_link_libraries(
	ign_core_tests
	ign_core
	boost_unit_test_framework
) 

# Add the test.
add_test(
	NAME ign_core_tests
	COMMAND ign_core_tests
)
