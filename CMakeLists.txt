cmake_minimum_required (VERSION 3.20)

project ("TestGetComputerName")

# Add source to this project's executable.
add_executable (TestGetComputerName "main.cpp" )

set_property(TARGET TestGetComputerName PROPERTY CXX_STANDARD 20)

target_link_libraries(TestGetComputerName PUBLIC iphlpapi)
