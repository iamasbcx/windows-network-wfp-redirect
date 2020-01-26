# CMakeList.txt : CMake project for net-proxy, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("net-proxy")

# Add source to this project's executable.
add_executable (net-proxy "net-proxy.cpp" "net-proxy.h")

# TODO: Add tests and install targets if needed.
