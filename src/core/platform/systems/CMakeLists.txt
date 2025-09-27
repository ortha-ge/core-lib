
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GLFWSystem.cppm
)

target_sources(core-lib PRIVATE
        GLFWSystem.cpp
)
