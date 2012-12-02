#
# version_git.cmake: CMake custom commands for git-based version information.
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

# Make sure we can find git.
find_program(GIT_EXECUTABLE git)
if(NOT GIT_EXECUTABLE)
    message(FATAL_ERROR "Unable to find 'git'")
endif(NOT GIT_EXECUTABLE)

# Fetch the current git branch name.
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    OUTPUT_VARIABLE GIT_BRANCH_NAME
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Fetch the last git commit hash.
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    OUTPUT_VARIABLE GIT_BRANCH_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Create an intermediate file with the new contents.
file(
    WRITE ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    "/*
 * version.h: An automatically-generated git-based version definition file.
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

#define VERSION \"@TARGET_VERSION@.@GIT_BRANCH_HASH@-@GIT_BRANCH_NAME@\"\n"
)

# Copy over the current file, but only if its different!
execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in 
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
)

# Remove the intermediate file.
execute_process(
    COMMAND ${CMAKE_COMMAND} -E remove 
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
)
