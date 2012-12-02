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
    ign_locate
    main.cpp
)

# Link to required libraries
target_link_libraries(
    ign_locate
    ign_directories
)

# Configure the automatic version header generation.
set(TARGET_VERSION 0.1)
add_custom_target(ign_locate_version ALL
    COMMAND ${CMAKE_COMMAND}
    -D TARGET_VERSION=${TARGET_VERSION}
    -P ${PROJECT_SOURCE_DIR}/cmake/version_git.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_dependencies(ign_locate ign_locate_version)

# Specify the install location.
install(
    TARGETS ign_locate
    DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
)
