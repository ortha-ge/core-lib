module;

#include <memory>
#include <string>

#include <entt/fwd.hpp>

export module Core.ResourceHandle;

import Core.Resource;

export namespace Core {

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

} // namespace Core
