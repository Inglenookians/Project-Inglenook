# Make the library.
add_executable(
	ign_locate
	main.cpp
)

# Link to required libraries
target_link_libraries(
	ign_locate
	ign_directories
	boost_program_options
)

# Specify the install location.
install(TARGETS ign_locate DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

