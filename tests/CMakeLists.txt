# test/CMakeLists.txt

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(TESTS architecture_tests individual_tests simulation_tests)

foreach(TEST IN LISTS TESTS)
	add_executable(${TEST} ${CMAKE_SOURCE_DIR}/tests/${TEST}.cpp)
	target_include_directories(${TEST} PRIVATE ${CMAKE_SOURE_DIR})
	target_link_libraries(${TEST} PUBLIC Boost::unit_test_framework)
	set_target_properties(${TEST} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/test/$<0:>)
endforeach()
