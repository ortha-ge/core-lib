module;

#include <memory>

export module Core.ResourceLoadRequest;

import Core.ResourceHandle;

export namespace Core {

	class ResourceLoadRequest {
	public:

		ResourceLoadRequest(std::shared_ptr<ResourceHandle>);

		std::shared_ptr<ResourceHandle> lockResourceHandle() const;

	private:

		std::weak_ptr<ResourceHandle> mResourceHandle;

	};

} // namespace Core
