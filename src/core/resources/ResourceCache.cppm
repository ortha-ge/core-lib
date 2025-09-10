module;

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include <entt/fwd.hpp>

export module Core.ResourceCache;

import Core.Resource;

export namespace Core {

	class ResourceCache {
	public:
		std::shared_ptr<Resource> getResource(const std::string&) const;
		std::shared_ptr<Resource> addResource(const std::string&, entt::entity);

		void cleanupLeastUsedResources(entt::registry&);
		void updateRecentlyUsedResources(std::shared_ptr<Resource> resource);

		// LRU for determining resources based on how recently a resource has been requested.
		// Resources use a shared handle so they don't get released unless all handles are dropped.
		// Resources are mapped to identifier so multiple requests will fetch the shared version.
		// 'Load<T>' call will automagically create adapter.
		// Adapters could be mapped to TypeId with a register call for runtime resolution.

	private:
		std::unordered_map<std::string, std::weak_ptr<Resource>> mResourceLookup{};
		std::list<std::shared_ptr<Resource>> mRecentlyUsedResources{};
	};

} // namespace Core
