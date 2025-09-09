module;

#include <memory>
#include <string>

#include <entt/entity/entity.hpp>

module Core.ResourceHandle;

namespace Core {

	Resource::Resource(std::string filePath, const entt::entity resourceEntity)
		: mFilePath{ std::move(filePath) }
		, mResourceEntity{ resourceEntity } {
	}

	const std::string& Resource::getFilePath() const {
		return mFilePath;
	}

	entt::entity Resource::getResourceEntity() const {
		return mResourceEntity;
	}

	ResourceHandle::ResourceHandle() = default;
	ResourceHandle::ResourceHandle(std::shared_ptr<Resource> resource)
		: mResource{ std::move(resource) } {}

	ResourceHandle::~ResourceHandle() = default;

	ResourceHandle::ResourceHandle(const ResourceHandle& other) = default;
	ResourceHandle::ResourceHandle(ResourceHandle&& other) noexcept = default;
	ResourceHandle& ResourceHandle::operator=(const ResourceHandle& other) = default;
	ResourceHandle& ResourceHandle::operator=(ResourceHandle&& other) noexcept = default;

	entt::entity ResourceHandle::getResourceEntity() const {
		return mResource ? mResource->getResourceEntity() : entt::null;
	}

} // namespace Core
