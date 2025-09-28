module;

#include <chrono>
#include <memory>
#include <utility>

#include <tracy/Tracy.hpp>

module Core.ResourceLoadSystem;

import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.Log;
import Core.ProcessError;
import Core.Resource;
import Core.ResourceCache;
import Core.ResourceHandle;
import Core.ResourceLoadRequest;
import entt;

namespace Core {

	ResourceLoadSystem::ResourceLoadSystem(EnTTRegistry& _registry, Scheduler& scheduler)
		: mRegistry{ _registry }
		, mScheduler{ scheduler } {

		entt::registry& registry(mRegistry);
		mResourceCacheEntity = registry.create();
		registry.emplace<ResourceCache>(mResourceCacheEntity);

		mTickHandle = mScheduler.schedule([this]() { tick(mRegistry); });
	}

	ResourceLoadSystem::~ResourceLoadSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void ResourceLoadSystem::tick(entt::registry& registry) {
		ZoneScopedN("ResourceLoadSystem::tick");

		auto& resourceCache{ registry.get<ResourceCache>(mResourceCacheEntity) };

		auto resourceLoadRequestView = registry.view<ResourceLoadRequest>(entt::exclude<ProcessError>);
		resourceLoadRequestView.each([&](entt::entity entity, const ResourceLoadRequest& loadRequest) {
			auto resourceHandle = loadRequest.lockResourceHandle();
			if (!resourceHandle) {
				registry.destroy(entity);
				return;
			}

			const std::string& resourceFilePath{ resourceHandle->getResourceFilePath() };
			if (resourceFilePath.empty()) {
				addProcessError(registry, entity, "Empty resource file path.");
				return;
			}

			auto resource{ resourceCache.getResource(resourceFilePath) };
			if (!resource) {
				entt::entity _resourceEntity{ loadRequest.createResource(registry) };
				registry.emplace<FileDescriptor>(_resourceEntity, resourceFilePath);
				registry.emplace<FileLoadRequest>(_resourceEntity);

				resource = resourceCache.addResource(resourceFilePath, _resourceEntity);
			}

			resourceHandle->setResource(resource);

			resourceCache.updateRecentlyUsedResources(resource);
			registry.destroy(entity);
		});

		if (resourceCache.isNextCleanupTimePassed()) {
			resourceCache.cleanupLeastUsedResources(registry);
		}
	}

} // namespace Core
