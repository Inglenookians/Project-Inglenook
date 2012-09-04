# Make the library.
add_executable(
	ist-locate
	main.cpp
)

# Link to required libraries
target_link_libraries(
	ist-locate
	ist
)

# Specify the install location.
install(TARGETS ist-locate DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

