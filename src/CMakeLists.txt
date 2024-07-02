# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Make an automatic library - will be static or dynamic based on user setting
file(GLOB sources *.hpp *.cpp)

add_library(libIRC SHARED ${sources} ${HEADER_LIST})

target_include_directories(libIRC PUBLIC ./)
# We need this directory, and users of our library will need it too

# All users of this library will need at least C++11
target_compile_features(libIRC PUBLIC cxx_std_98)

# IDEs should put the headers in a nice place
# source_group(
#   TREE "${PROJECT_SOURCE_DIR}/include"
#   PREFIX "Header Files"
#   FILES ${HEADER_LIST})