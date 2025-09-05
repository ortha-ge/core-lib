module;

#include <entt/entity/entity.hpp>

module Core.ResourceHandle;

namespace Core {

	ResourceHandle::ResourceHandle() = default;
	ResourceHandle::ResourceHandle(entt::entity resourceEntity)
		: mResourceEntity(resourceEntity) {}

	ResourceHandle::~ResourceHandle() = default;

	ResourceHandle::ResourceHandle(const ResourceHandle& other) = default;
	ResourceHandle::ResourceHandle(ResourceHandle&& other) noexcept = default;
	ResourceHandle& ResourceHandle::operator=(const ResourceHandle& other) = default;
	ResourceHandle& ResourceHandle::operator=(ResourceHandle&& other) noexcept = default;

} // namespace Core
