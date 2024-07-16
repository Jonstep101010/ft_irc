# CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(IRC)

set(CLANG_TIDY_COMMAND "/usr/bin/clang-tidy -config=.clang-tidy")

# Set the C++ standard to C++98
set(CMAKE_CXX_STANDARD 98)

# Set the C++ compiler
set(CMAKE_CXX_COMPILER /usr/bin/clang++-17)

# Add the executable target
file(GLOB sources src/main.cpp)
add_executable(IRC "")
target_link_libraries(IRC PUBLIC libIRC)
target_include_directories(IRC PUBLIC ./src)
target_sources(IRC PRIVATE ${sources})
set_target_properties(IRC PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++98 -fno-limit-debug-info")
# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(src)

# if we want to add tests
# add_subdirectory(tests)