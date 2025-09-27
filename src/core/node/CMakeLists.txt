
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        EnTTNode.cppm
        Node.cppm
        TemplateNode.cppm
)

target_sources(core-lib PRIVATE
        EnTTNode.cpp
        Node.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
