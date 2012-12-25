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
    ign_directories
    SHARED
    directories.cpp
)

# Link to required libraries
target_link_libraries(
    ign_directories
    ign_core
)

# Set the properties
set_target_properties(
    ign_directories PROPERTIES
    VERSION 0.0.1
    SOVERSION 1
)

# Specify the install location.
install(
    TARGETS ign_directories
    DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

#########
# Tests #
#########

# Make the test.
add_executable(
    ign_directories_tests
    tests.cpp
)

# Set the properties
set_target_properties(
    ign_directories_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_OUTPUT_DIRECTORY}
)

# Link to required libraries
target_link_libraries(
    ign_directories_tests
    ign_directories
    boost_unit_test_framework
) 

# Add the test.
add_test(
    NAME ign_directories_tests
    COMMAND ign_directories_tests
)
