module;

#include <memory>
#include <string>

module Ortha.Core.ResourceHandle;

import entt;

namespace Core {

	ResourceHandle::ResourceHandle() = default;
	ResourceHandle::ResourceHandle(std::shared_ptr<Resource> resource)
		: mResource{ std::move(resource) } {}

	ResourceHandle::ResourceHandle(std::string resourceFilePath)
		: mResourceFilePath(std::move(resourceFilePath)) {
	}

	ResourceHandle::~ResourceHandle() = default;

	ResourceHandle::ResourceHandle(const ResourceHandle& other) = default;
	ResourceHandle::ResourceHandle(ResourceHandle&& other) noexcept = default;
	ResourceHandle& ResourceHandle::operator=(const ResourceHandle& other) = default;
	ResourceHandle& ResourceHandle::operator=(ResourceHandle&& other) noexcept = default;

	void ResourceHandle::setResource(std::shared_ptr<Resource> resource) {
		mResource = std::move(resource);
	}

	const std::string& ResourceHandle::getResourceFilePath() const {
		return mResourceFilePath;
	}

	entt::entity ResourceHandle::getResourceEntity() const {
		return mResource ? mResource->getResourceEntity() : entt::null;
	}

} // namespace Core
