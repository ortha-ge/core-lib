
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GLFWWindow.cppm
        NativeWindowHandles.cppm
        QuitAppRequest.cppm
        Window.cppm
)

target_sources(core-lib PRIVATE
)

add_subdirectory(services)
add_subdirectory(systems)
