module;

#include <memory>
#include <tuple>

#include <entt/entt.hpp>

export module Core.ResourceHandle;

import Core.Resource;

export namespace Core {

	class ResourceHandle {
	public:
		ResourceHandle();
		explicit ResourceHandle(std::shared_ptr<Resource> resource);
		ResourceHandle(std::string resourceFilePath);
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
