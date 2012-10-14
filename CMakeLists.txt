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

cmake_minimum_required (VERSION 2.8)

#
# Project Settings
#
message(STATUS "Configuring project settings")

# Set the project name and versioning details
project(PROJECT_INGLENOOK)
set(PROJECT_INGLENOOK_VERSION_MAJOR 0)
set(PROJECT_INGLENOOK_VERSION_MINOR 0)
set(PROJECT_INGLENOOK_VERSION_PATCH 1)
set(PROJECT_INGLENOOK_VERSION_SPECIALTAG "")

message("Project")
message("	Project-Inglenook	v${PROJECT_INGLENOOK_VERSION_MAJOR}.${PROJECT_INGLENOOK_VERSION_MINOR}.${PROJECT_INGLENOOK_VERSION_PATCH}${PROJECT_INGLENOOK_VERSION_SPECIALTAG}")


# Check we are not performing an in-source build.
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" INSOURCE)
if(INSOURCE)
	message(FATAL_ERROR "${PROJECT_NAME} requires an out of source build. Please use the 'setup_builds.pl' script in the project root directory to create separate build directories and run make there. Beware, this will have automatically generated the CMakeCache.txt and CMakeFiles directories!" )
endif(INSOURCE)


# Setup build type if not already set.
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE debug)
endif(NOT CMAKE_BUILD_TYPE)
message("	Build Type		${CMAKE_BUILD_TYPE}")
message("	Build Directory		${CMAKE_BINARY_DIR}")

# Setup build directories if not already set.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(MAN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/man)

# Setup the default GNU installation directories
include(GNUInstallDirs)
message("Installation destinations")
message("	\${CMAKE_INSTALL_FULL_BINDIR}     \t ${CMAKE_INSTALL_FULL_BINDIR}     \t (CLI Binaries)")
message("	\${CMAKE_INSTALL_FULL_SBINDIR}    \t ${CMAKE_INSTALL_FULL_SBINDIR}    \t (Server Binaries)")
message("	\${CMAKE_INSTALL_FULL_LIBDIR}     \t ${CMAKE_INSTALL_FULL_LIBDIR}     \t (Dynamic/Static Libraries)")
message("	\${CMAKE_INSTALL_FULL_INCLUDEDIR} \t ${CMAKE_INSTALL_FULL_INCLUDEDIR} \t (Header Includes)")
message("	\${CMAKE_INSTALL_FULL_MANDIR}     \t ${CMAKE_INSTALL_FULL_MANDIR}    (Manual Pages)")

# Setup the compiler to use c++11
message(STATUS "Configuring c++11 support")
ADD_DEFINITIONS("-std=c++11")


#
# Testing (use 'make test')
#
message(STATUS "Configuring testing")
enable_testing()


#
# Source code
#
message(STATUS "Configuring source code")
# Add the subdirectories to scan for include headers.
include_directories(src/lib)

# Add RPATH for install based on CMAKE_INSTALL_FULL_LIBDIR
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})
# Add the subdirectories to scan for CMakeFiles.txt to build.
add_subdirectory(src)


#
# Manual Pages
#
# Create manpage generation instructions.
message(STATUS "Configuring manual pages")

# Search for required cat, sed and gzip programs.
find_program(CAT_EXECUTABLE cat)
if(NOT CAT_EXECUTABLE)
	message(FATAL_ERROR "Unable to find 'cat'")
endif(NOT CAT_EXECUTABLE)
find_program(SED_EXECUTABLE sed)
if(NOT SED_EXECUTABLE)
	message(FATAL_ERROR "Unable to find 'sed'")
endif(NOT SED_EXECUTABLE)
find_program(GZIP_EXECUTABLE gzip)
if(NOT GZIP_EXECUTABLE)
	message(FATAL_ERROR "Unable to find 'gzip'")
endif(NOT GZIP_EXECUTABLE)
find_program(DATE_EXECUTABLE date)
if(NOT DATE_EXECUTABLE)
	message(FATAL_ERROR "Unable to find 'date'")
endif(NOT DATE_EXECUTABLE)

# Loop through each manpage section.
execute_process(
	COMMAND ${DATE_EXECUTABLE} +"%d %B, %Y"
	OUTPUT_VARIABLE MANPAGE_DATE
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
foreach(MANPAGE_NUM RANGE 1 9)

	# Search for any files that match the following pattern '*.${MANPAGE_NUM}.manpage'
	file(GLOB_RECURSE MANPAGE_FOUND ${CMAKE_SOURCE_DIR}/src/*.${MANPAGE_NUM}.manpage)

	# Loop through each manpage file found. 
	foreach(MANPAGE_FILE ${MANPAGE_FOUND})

		# Fetch the filename from the full file page.
		get_filename_component(MANPAGE_FILENAME ${MANPAGE_FILE} NAME)

		# Generate the required output filename and the manpage name.
		string(REPLACE ".manpage" ".gz" MANPAGE_GZ ${MANPAGE_FILENAME})
		string(REPLACE ".${MANPAGE_NUM}.manpage" "" MANPAGE_NAME ${MANPAGE_FILENAME})
		message("	${MANPAGE_FILENAME}")

		# Add a custom command to build the manpage when 'make' is called. 
		add_custom_command(
			OUTPUT  ${MAN_OUTPUT_DIRECTORY}/${MANPAGE_GZ}
			COMMAND ${CAT_EXECUTABLE} ${MANPAGE_FILE}
				| ${SED_EXECUTABLE} s/IGNMAN.NAME/${MANPAGE_NAME}/g
				| ${SED_EXECUTABLE} s/IGNMAN.SECTION/${MANPAGE_NUM}/g
				| ${SED_EXECUTABLE} s/IGNMAN.DATE/${MANPAGE_DATE}/g
				| ${GZIP_EXECUTABLE} -qc > ${MAN_OUTPUT_DIRECTORY}/${MANPAGE_GZ}
			DEPENDS ${MANPAGE_FILE}
			COMMENT "Generating manual page '${MANPAGE_GZ}'"
		)

		# Add a rule for 'make install' to install the man page into the current location.
		install(FILES ${MAN_OUTPUT_DIRECTORY}/${MANPAGE_GZ} DESTINATION ${CMAKE_INSTALL_FULL_MANDIR}/man${MANPAGE_NUM}/)

		# Create a list of manpage output names, required lower down. 
		list(APPEND MANPAGE_LIST ${MAN_OUTPUT_DIRECTORY}/${MANPAGE_GZ})

	endforeach(MANPAGE_FILE)

endforeach(MANPAGE_NUM)

# Ensure that the man build directory exists before creating the target.
execute_process(COMMAND mkdir -p ${MAN_OUTPUT_DIRECTORY})
# Add a custom target that required that all listed manpgaes processed above are built.
add_custom_target("manual pages" ALL echo
	DEPENDS ${MANPAGE_LIST}
)


#
# Other
#


#
# CPack Options
#
message(STATUS "Configuring cpack")

include (InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "Project-Inglenook")
set(CPACK_PACKAGE_VENDOR "Inglenookians")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_INGLENOOK_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_INGLENOOK_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_INGLENOOK_VERSION_PATCH}${PROJECT_INGLENOOK_VERSION_SPECIALTAG}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Facilities automation and integration project")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION")
#set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
#set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
#set(CPACK_STRIP_FILES abc.txt 123.txt)
include (CPack)

