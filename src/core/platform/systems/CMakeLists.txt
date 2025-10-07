
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GLFWSystem.cppm
)

target_sources(ortha-core PRIVATE
        GLFWSystem.cpp
)
