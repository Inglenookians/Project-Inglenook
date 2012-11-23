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

# Make the application.
add_executable(
	ign_log_write
	main.cpp
	log_write_actions.cpp
)

# Link to required libraries
target_link_libraries(
	ign_log_write
	ign_logging
	ign_core
)

# Configure the automatic version header generation.
set(TARGET_VERSION 0.1)
add_custom_target(ign_log_write_version ALL 
	COMMAND ${CMAKE_COMMAND} 
	-D TARGET_VERSION=${TARGET_VERSION}
	-P ${PROJECT_SOURCE_DIR}/cmake/version_git.cmake
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_dependencies(ign_log_write ign_log_write_version)

# Specify the install location.
install(
	TARGETS ign_log_write 
	DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
)

###########
# Scripts #
###########

# Copy script into build directory.
add_custom_target(ign_logging.sh
	COMMAND cmake -E copy
	${CMAKE_CURRENT_SOURCE_DIR}/ign_logging.sh
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ign_logging.sh
)
add_dependencies(ign_log_write ign_logging.sh)

# Specify the install location.
install(
	FILES ign_logging.sh 
	DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
)

#########
# Tests #
#########

# Make the test.
add_executable(
	ign_log_write_tests 
	tests.cpp
	log_write_actions.cpp
)

# Set the properties
set_target_properties(
	ign_log_write_tests PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIRECTORY}
)

# Link to required libraries
target_link_libraries(
	ign_log_write_tests
	ign_logging
	ign_core
	boost_unit_test_framework
	boost_regex
) 

# Add the test.
add_test(
	NAME ign_log_write_tests
	COMMAND ign_log_write_tests
)
