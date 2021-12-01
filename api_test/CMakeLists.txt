cmake_minimum_required (VERSION 3.8)

add_executable (api_test "main.cpp")
target_link_libraries (api_test PUBLIC rule34xxx_cpp_api)
set_property(TARGET api_test PROPERTY CXX_STANDARD 20)