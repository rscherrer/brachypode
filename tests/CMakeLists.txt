# tests/CMakeLists.txt

# Find Boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Model 'unit' files
file(GLOB_RECURSE unit ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Include test utilities
include_directories(${CMAKE_SOURCE_DIR}/tests)

# Names of the test executables
set(TESTS tests population_tests architecture_tests individual_tests buffer_tests printer_tests utilities_tests parameters_tests)

# Build each executable
foreach(TEST IN LISTS TESTS)
	add_executable(${TEST} ${CMAKE_SOURCE_DIR}/tests/${TEST}.cpp ${unit} ${CMAKE_SOURCE_DIR}/tests/testutils.cpp)
	target_include_directories(${TEST} PRIVATE ${CMAKE_SOURE_DIR} ${CMAKE_SOURCE_DIR}/tests)
	target_link_libraries(${TEST} PUBLIC Boost::unit_test_framework)
	set_target_properties(${TEST} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/tests/$<0:>)
endforeach()