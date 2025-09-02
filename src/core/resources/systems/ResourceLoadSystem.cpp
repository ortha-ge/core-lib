module;

#include <utility>

#include <entt/entt.hpp>

module Core.ResourceLoadSystem;

import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.ResourceCache;
import Core.ResourceHandle;
import Core.ResourceLoadRequest;

namespace Core {

	ResourceLoadSystem::ResourceLoadSystem(EnTTRegistry& _registry, Scheduler& scheduler)
		: mRegistry{ _registry }
		, mScheduler{ scheduler } {

		entt::registry& registry(mRegistry);
		mResourceCacheEntity = registry.create();
		registry.emplace<ResourceCache>(mResourceCacheEntity);

		mTickHandle = mScheduler.schedule([this]() {
			tick(mRegistry);
		});
	}

	ResourceLoadSystem::~ResourceLoadSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void ResourceLoadSystem::tick(entt::registry& registry) {
		auto& resourceCache{ registry.get<ResourceCache>(mResourceCacheEntity) };

		auto resourceLoadRequestView = registry.view<ResourceLoadRequest>();
		resourceLoadRequestView.each([&](entt::entity entity, const ResourceLoadRequest& loadRequest) {
			const std::string& resourceFilePath{ loadRequest.getResourceFilePath() };
			auto resourceEntity{ resourceCache.getResource(resourceFilePath) };
			if (!resourceEntity) {
				printf("Resource not already loaded: %s\n", resourceFilePath.c_str());

				entt::entity _resourceEntity{ loadRequest.createResource(registry) };
				registry.emplace<FileDescriptor>(_resourceEntity, resourceFilePath);
				registry.emplace<FileLoadRequest>(_resourceEntity);

				resourceEntity = _resourceEntity;
				resourceCache.addResource(resourceFilePath, _resourceEntity);
			} else {
				printf("Resource already loaded: %s\n", resourceFilePath.c_str());
			}

			registry.emplace<ResourceHandle>(entity, *resourceEntity);

			resourceCache.updateRecentlyUsedResources(*resourceEntity);
			registry.remove<ResourceLoadRequest>(entity);
		});

		const bool shouldFreeLRU{ false };
		if (shouldFreeLRU) {
			resourceCache.cleanupLeastUsedResources();
		}
	}

} // namespace Core
