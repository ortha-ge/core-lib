module;

#include <algorithm>
#include <optional>

#include <entt/entity/entity.hpp>

module Core.ResourceCache;

namespace Core {

	std::optional<entt::entity> ResourceCache::getResource(const std::string& resourceFilePath) const {
		if (auto it = mResourceLookup.find(resourceFilePath); it != mResourceLookup.end()) {
			return it->second;
		}

		return std::nullopt;
	}

	void ResourceCache::addResource(const std::string& resourceFilePath, entt::entity resourceEntity) {
		mResourceLookup[resourceFilePath] = resourceEntity;
	}

	void ResourceCache::cleanupLeastUsedResources() {}

	void ResourceCache::updateRecentlyUsedResources(entt::entity resourceEntity) {
		std::ranges::remove(mRecentlyUsedResources, resourceEntity);
		mRecentlyUsedResources.push_back(resourceEntity);
	}

} // namespace Core
