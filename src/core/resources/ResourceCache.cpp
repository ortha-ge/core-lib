module;

#include <algorithm>
#include <chrono>
#include <string>

module Core.ResourceCache;

import Core.Log;
import Core.Resource;
import entt;

namespace Core {

	ResourceCache::ResourceCache()
		: mCleanupTriggerTimeSeconds(5) {
	}

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
				logEntry(registry, "Cleaning up resource: {}", resource->getFilePath());
				registry.destroy(resource->getResourceEntity());
				mResourceLookup.erase(resource->getFilePath());
				mRecentlyUsedResources.erase(currentIt);
			}
		}

		mLastCleanupTime = std::chrono::steady_clock::now();
	}

	void ResourceCache::updateRecentlyUsedResources(std::shared_ptr<Resource> resource) {
		auto it = std::ranges::remove(mRecentlyUsedResources, resource);
		mRecentlyUsedResources.erase(it.begin(), it.end());
		mRecentlyUsedResources.push_back(std::move(resource));
	}

	void ResourceCache::setCleanupTriggerTimeSeconds(const std::chrono::seconds& timeInSeconds) {
		mCleanupTriggerTimeSeconds = timeInSeconds;
	}

	const std::chrono::seconds& ResourceCache::getCleanupTriggerTimeSeconds() const {
		return mCleanupTriggerTimeSeconds;
	}

	bool ResourceCache::isNextCleanupTimePassed() const {
		return std::chrono::steady_clock::now() >= (mLastCleanupTime + mCleanupTriggerTimeSeconds);
	}

	const ResourceCache::ResourceMap& ResourceCache::getResourceLookup() const {
		return mResourceLookup;
	}


} // namespace Core
