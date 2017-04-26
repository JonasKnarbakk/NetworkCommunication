INCLUDE_DIRECTORIES(${NetworkCommunication_SOURCE_DIR}/Server/headers)
INCLUDE_DIRECTORIES(${NetworkCommunication_SOURCE_DIR}/Server/src)

ADD_EXECUTABLE(Server main.cpp)

SET_TARGET_PROPERTIES( Server
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${NetworkCommunication_SOURCE_DIR}/bin
    )
