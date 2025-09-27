
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        FileLoadRequest.cppm
        RawDataResource.cppm
        Resource.cppm
        ResourceCache.cppm
        ResourceHandle.cppm
        ResourceHandleUtils.cppm
        ResourceLoadRequest.cppm
)

target_sources(core-lib PRIVATE
        Resource.cpp
        ResourceCache.cpp
        ResourceHandle.cpp
        ResourceLoadRequest.cpp
)

add_subdirectory(descriptors)
add_subdirectory(services)
add_subdirectory(systems)
