# Make the library.
add_library(
	ist 
	SHARED
	Config.cpp
	Directories.cpp
	Environment.cpp
)

# Set the properties
set_target_properties(
	ist PROPERTIES
	VERSION 1.0.0
	SOVERSION 1
)

# Specify the install location.
install(TARGETS ist DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})