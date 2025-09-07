module;

#include <format>
#include <chrono>
#include <utility>
#include <string_view>

#include <entt/entt.hpp>

module Core.ResourceLoadSystem;

import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.Log;
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

		mNextCleanupTime = std::chrono::steady_clock::now() + std::chrono::seconds(5);
		mTickHandle = mScheduler.schedule([this]() { tick(mRegistry); });
	}

	ResourceLoadSystem::~ResourceLoadSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void ResourceLoadSystem::tick(entt::registry& registry) {
		auto& resourceCache{ registry.get<ResourceCache>(mResourceCacheEntity) };

		auto resourceLoadRequestView = registry.view<ResourceLoadRequest>();
		resourceLoadRequestView.each([&](entt::entity entity, const ResourceLoadRequest& loadRequest) {
			const std::string& resourceFilePath{ loadRequest.getResourceFilePath() };
			auto resource{ resourceCache.getResource(resourceFilePath) };
			if (!resource) {
				//logEntry(registry, entity, "Resource not already loaded: {}", resourceFilePath);

				entt::entity _resourceEntity{ loadRequest.createResource(registry) };
				registry.emplace<FileDescriptor>(_resourceEntity, resourceFilePath);
				registry.emplace<FileLoadRequest>(_resourceEntity);

				resource = resourceCache.addResource(resourceFilePath, _resourceEntity);
			} else {
				//logEntry(registry, entity, "Resource already loaded: {}", resourceFilePath);
			}

			registry.emplace<ResourceHandle>(entity, resource);

			resourceCache.updateRecentlyUsedResources(resource);
			registry.remove<ResourceLoadRequest>(entity);
		});

		const bool shouldFreeLRU{ std::chrono::steady_clock::now() >= mNextCleanupTime };
		if (shouldFreeLRU) {
			resourceCache.cleanupLeastUsedResources(registry);
			mNextCleanupTime = std::chrono::steady_clock::now() + std::chrono::seconds(5);
		}
	}

} // namespace Core
