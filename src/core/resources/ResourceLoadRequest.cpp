module;

#include <memory>

module Core.ResourceLoadRequest;

namespace Core {

	ResourceLoadRequest::ResourceLoadRequest(std::shared_ptr<ResourceHandle> resourceHandle)
		: mResourceHandle(resourceHandle) {
	}

	std::shared_ptr<ResourceHandle> ResourceLoadRequest::lockResourceHandle() const {
		return mResourceHandle.lock();
	}

} // namespace Core
