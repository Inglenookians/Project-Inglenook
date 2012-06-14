# Make the library.
add_library(
	ild 
	SHARED
	ILD.cpp
)

# Set the properties
set_target_properties(
	ild PROPERTIES
	VERSION 1.0.0
	SOVERSION 1
)

# Specify the install location.
install(TARGETS ild DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

