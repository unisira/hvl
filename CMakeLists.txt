cmake_minimum_required(VERSION 3.0)

project(improvisor)

# Find the WDK package
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(WDK REQUIRED)

add_subdirectory(hvl)
add_subdirectory(examples)
add_subdirectory(tests)
