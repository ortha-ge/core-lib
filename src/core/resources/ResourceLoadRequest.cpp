module;

#include <memory>

#include <entt/entt.hpp>

module Core.ResourceLoadRequest;

namespace Core {

	ResourceLoadRequest::ResourceLoadRequest(std::shared_ptr<ResourceHandle> resourceHandle, TypeLoaderFunction typeLoaderFunc)
		: mResourceHandle(resourceHandle)
		, mTypeLoaderFunc(std::move(typeLoaderFunc)) {
	}

	std::shared_ptr<ResourceHandle> ResourceLoadRequest::lockResourceHandle() const {
		return mResourceHandle.lock();
	}

	entt::entity ResourceLoadRequest::createResource(entt::registry& registry) const {
		const entt::entity resourceEntity{ registry.create() };
		mTypeLoaderFunc(registry, resourceEntity);
		return resourceEntity;
	}

} // namespace Core
