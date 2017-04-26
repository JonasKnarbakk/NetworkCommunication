# Name of the prefix of this variable is set by CMake
# according to the value given with "PROJECT()" in the root CMakeLists.txt
# Example of including a library

# INCLUDE_DIRECTORIES(${MyProject_SOURCE_DIR}/Library/inc)
# ADD_LIBRARY(Library file.cpp)

INCLUDE_DIRECTORIES(${NetworkCommunication_SOURCE_DIR}/Client/headers)
INCLUDE_DIRECTORIES(${NetworkCommunication_SOURCE_DIR}/Client/src)

ADD_EXECUTABLE(Client main.cpp)

SET_TARGET_PROPERTIES( Client
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${NetworkCommunication_SOURCE_DIR}/bin
    )
#INSTALL (TARGETS Client DESTINATION bin)
