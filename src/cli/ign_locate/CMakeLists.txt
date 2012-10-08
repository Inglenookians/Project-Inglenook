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

# Make the library.
add_executable(
	ign_locate
	main.cpp
)

# Link to required libraries
target_link_libraries(
	ign_locate
	ign_directories
)

# Specify the install location.
install(TARGETS ign_locate DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

