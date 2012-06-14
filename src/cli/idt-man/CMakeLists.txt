# Make the library.
add_executable(
	idt-man
	main.cpp
)

# Link to required libraries
target_link_libraries(
	idt-man
	ild
)

# Specify the install location.
install(TARGETS idt-man DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

