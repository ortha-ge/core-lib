module;

#include <memory>
#include <tuple>

#include <entt/entt.hpp>

export module Core.ResourceHandleUtils;

import Core.ResourceHandle;

export namespace Core {

	template <typename T>
		std::tuple<entt::entity, T*> getResourceAndEntity(entt::registry& registry, const std::shared_ptr<ResourceHandle>& resourceHandle) {
		if (!resourceHandle) {
			return { entt::null, nullptr };
		}

		const entt::entity resourceEntity{ resourceHandle->getResourceEntity() };
		if (!registry.all_of<T>(resourceEntity)) {
			return { resourceEntity, nullptr };
		}

		return { resourceEntity, &registry.get<T>(resourceEntity) };
	}

	template <typename T>
	T* getResource(entt::registry& registry, const std::shared_ptr<ResourceHandle>& resourceHandle) {
		return std::get<1>(getResourceAndEntity<T>(registry, resourceHandle));
	}

} // namespace Core
