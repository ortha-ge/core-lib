
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        EntityVisitor.cppm
)

target_sources(ortha-core PRIVATE
)

add_subdirectory(services)
add_subdirectory(systems)
