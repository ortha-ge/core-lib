module;

#include <algorithm>
#include <string>

#include <entt/entity/entity.hpp>

module Core.ResourceCache;

namespace Core {

	std::shared_ptr<Resource> ResourceCache::getResource(const std::string& resourceFilePath) const {
		if (auto it = mResourceLookup.find(resourceFilePath); it != mResourceLookup.end()) {
			return it->second.lock();
		}

		return {};
	}

	std::shared_ptr<Resource> ResourceCache::addResource(const std::string& resourceFilePath, entt::entity resourceEntity) {
		auto resource = std::make_shared<Resource>(resourceFilePath, resourceEntity);
		mResourceLookup[resourceFilePath] = resource;
		return resource;
	}

	void ResourceCache::cleanupLeastUsedResources(entt::registry& registry) {
		for (auto it = mRecentlyUsedResources.begin(); it != mRecentlyUsedResources.end();) {
			auto currentIt = it++;
			const auto& resource = *currentIt;
			if (currentIt->use_count() == 1) {
				registry.destroy(resource->getResourceEntity());
				mResourceLookup.erase(resource->getFilePath());
				mRecentlyUsedResources.erase(currentIt);
			}
		}
	}

	void ResourceCache::updateRecentlyUsedResources(std::shared_ptr<Resource> resource) {
		auto it = std::ranges::remove(mRecentlyUsedResources, resource);
		mRecentlyUsedResources.erase(it.begin(), it.end());
		mRecentlyUsedResources.push_back(std::move(resource));
	}

} // namespace Core
