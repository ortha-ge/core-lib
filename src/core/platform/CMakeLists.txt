
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GLFWWindow.cppm
        NativeWindowHandles.cppm
        QuitAppRequest.cppm
        Window.cppm
)

target_sources(ortha-core PRIVATE
)

add_subdirectory(services)
add_subdirectory(systems)
