cmake_minimum_required (VERSION 3.8)

find_package(CURL CONFIG REQUIRED)

add_library (rule34xxx_cpp_api "src/rule34xxx_cpp_api.cpp" "include/rule34xxx_cpp_api.h" "include/json.hpp")
target_include_directories(rule34xxx_cpp_api PUBLIC "include")
set_property(TARGET rule34xxx_cpp_api PROPERTY CXX_STANDARD 20)

target_link_libraries(rule34xxx_cpp_api PRIVATE CURL::libcurl)