module;

#include <list>
#include <optional>
#include <string>
#include <unordered_map>

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>

export module Core.ResourceCache;

export namespace Core {

	class ResourceCache {
	public:

		std::optional<entt::entity> getResource(const std::string& resourceFilePath) const;
		void addResource(const std::string& resourceFilePath, entt::entity resourceEntity);

		void cleanupLeastUsedResources();
		void updateRecentlyUsedResources(entt::entity resourceEntity);

		// LRU for determining resources based on how recently a resource has been requested.
		// Resources use a shared handle so they don't get released unless all handles are dropped.
		// Resources are mapped to identifier so multiple requests will fetch the shared version.
		// 'Load<T>' call will automagically create adapter.
		// Adapters could be mapped to TypeId with a register call for runtime resolution.

	private:

		std::unordered_map<std::string, entt::entity> mResourceLookup{};
		std::list<entt::entity> mRecentlyUsedResources{};

	};

} // namespace Core
