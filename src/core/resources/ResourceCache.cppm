module;

#include <chrono>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>

export module Core.ResourceCache;

import Core.Resource;
import entt;

export namespace Core {

	class ResourceCache {
	public:

		using ResourceMap = std::unordered_map<std::string, std::weak_ptr<Resource>>;

		ResourceCache();

		std::shared_ptr<Resource> getResource(const std::string&) const;
		std::shared_ptr<Resource> addResource(const std::string&, entt::entity);

		void cleanupLeastUsedResources(entt::registry&);
		void updateRecentlyUsedResources(std::shared_ptr<Resource> resource);

		void setCleanupTriggerTimeSeconds(const std::chrono::seconds& timeInSeconds);
		const std::chrono::seconds& getCleanupTriggerTimeSeconds() const;
		bool isNextCleanupTimePassed() const;

		const ResourceMap& getResourceLookup() const;

	private:
		ResourceMap mResourceLookup{};
		std::list<std::shared_ptr<Resource>> mRecentlyUsedResources{};
		std::chrono::steady_clock::time_point mLastCleanupTime{};
		std::chrono::seconds mCleanupTriggerTimeSeconds;
	};

} // namespace Core
