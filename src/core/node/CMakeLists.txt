
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        EnTTNode.cppm
        Node.cppm
        NodeHandle.cppm
)

target_sources(core-lib PRIVATE
        EnTTNode.cpp
        Node.cpp
        NodeHandle.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
