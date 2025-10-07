module;

#include <memory>
#include <string>

export module Ortha.Core.ResourceHandle;

import Ortha.Core.Resource;
import entt;

export namespace Ortha::Core {

	class ResourceHandle {
	public:
		ResourceHandle();
		explicit ResourceHandle(std::shared_ptr<Resource> resource);
		explicit ResourceHandle(std::string resourceFilePath);
		~ResourceHandle();

		ResourceHandle(const ResourceHandle& other);
		ResourceHandle(ResourceHandle&& other) noexcept;

		ResourceHandle& operator=(const ResourceHandle& other);
		ResourceHandle& operator=(ResourceHandle&& other) noexcept;

		void setResource(std::shared_ptr<Resource> resource);

		[[nodiscard]] const std::string& getResourceFilePath() const;
		[[nodiscard]] entt::entity getResourceEntity() const;

	private:

		std::string mResourceFilePath;
		std::shared_ptr<Resource> mResource;

	};

	using ResourceHandlePtr = std::shared_ptr<ResourceHandle>;

} // namespace Ortha::Core
