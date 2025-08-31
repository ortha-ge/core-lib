
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.FileLoadSystem;
import Core.RawDataResource;

TEST_CASE("FileDescriptorWithLoadRequest_FileLoadSystemTick_RawDataResourceProduced", "FileLoadSystem") {
    using namespace Core;
    entt::registry registry{};
    entt::entity fileResourceEntity = registry.create();
    registry.emplace<FileDescriptor>(fileResourceEntity, "./assets/TestFileResource.txt");
    registry.emplace<FileLoadRequest>(fileResourceEntity);

    FileLoadSystem::tickSystem(registry);

    const bool hasRawDataResource = registry.all_of<RawDataResource>(fileResourceEntity);
    REQUIRE(hasRawDataResource);
    const RawDataResource& rawDataResource = registry.get<RawDataResource>(fileResourceEntity);
    REQUIRE(rawDataResource.size == 10);
    REQUIRE(rawDataResource.rawData.capacity() == 10);
}
