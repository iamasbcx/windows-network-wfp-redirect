# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("windows-network-wfp-redirect-filter")


add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)


# Include sub-projects.
add_subdirectory ("network-wfp-redirect-filter")

