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

# Add all the examples.
add_executable(
    ign_examples_lib_logging_01_basic
    01-basic.cpp
)

add_executable(
    ign_examples_lib_logging_02_threading
    02-threading.cpp
)

add_executable(
    ign_examples_lib_logging_03_otr_and_fallback
    03-otr-and-fallback.cpp
)

# Set the properties
set_target_properties(
    ign_examples_lib_logging_01_basic PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIRECTORY}
)

set_target_properties(
    ign_examples_lib_logging_02_threading PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIRECTORY}
)

set_target_properties(
    ign_examples_lib_logging_03_otr_and_fallback PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIRECTORY}
)

# Link to required libraries
target_link_libraries(
    ign_examples_lib_logging_01_basic
    ign_logging
)

target_link_libraries(
    ign_examples_lib_logging_02_threading
    ign_logging
)

target_link_libraries(
    ign_examples_lib_logging_03_otr_and_fallback
    ign_logging
)

# Configure the automatic version header generation.
set(TARGET_VERSION 0.1)
add_custom_target(ign_examples_lib_logging_01_basic_version ALL
    COMMAND ${CMAKE_COMMAND}
    -D TARGET_VERSION=${TARGET_VERSION}
    -P ${PROJECT_SOURCE_DIR}/cmake/version_git.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(ign_examples_lib_logging_02_threading_version ALL
    COMMAND ${CMAKE_COMMAND}
    -D TARGET_VERSION=${TARGET_VERSION}
    -P ${PROJECT_SOURCE_DIR}/cmake/version_git.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(ign_examples_lib_logging_03_otr_and_fallback_version ALL
    COMMAND ${CMAKE_COMMAND}
    -D TARGET_VERSION=${TARGET_VERSION}
    -P ${PROJECT_SOURCE_DIR}/cmake/version_git.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(ign_examples_lib_logging_01_basic ign_examples_lib_logging_01_basic_version)

add_dependencies(ign_examples_lib_logging_02_threading ign_examples_lib_logging_02_threading_version)

add_dependencies(ign_examples_lib_logging_03_otr_and_fallback ign_examples_lib_logging_03_otr_and_fallback_version)
