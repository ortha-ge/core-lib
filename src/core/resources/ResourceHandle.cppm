module;

#include <entt/entt.hpp>
#include <memory>
#include <tuple>

export module Core.ResourceHandle;

export namespace Core {

	class Resource {
	public:

		explicit Resource(std::string filePath, entt::entity resourceEntity);

		[[nodiscard]] const std::string& getFilePath() const;
		[[nodiscard]] entt::entity getResourceEntity() const;

	private:

		std::string mFilePath;
		entt::entity mResourceEntity{ entt::null };

	};

	class ResourceHandle {
	public:
		ResourceHandle();
		explicit ResourceHandle(std::shared_ptr<Resource> resource);
		~ResourceHandle();

		ResourceHandle(const ResourceHandle& other);
		ResourceHandle(ResourceHandle&& other) noexcept;

		ResourceHandle& operator=(const ResourceHandle& other);
		ResourceHandle& operator=(ResourceHandle&& other) noexcept;

		[[nodiscard]] entt::entity getResourceEntity() const;

	private:

		std::shared_ptr<Resource> mResource;

	};

	template <typename T>
	std::tuple<entt::entity, T*> getResourceAndEntity(entt::registry& registry, const entt::entity entity) {
		if (!registry.all_of<ResourceHandle>(entity)) {
			return { entt::null, nullptr };
		}

		const auto& resourceHandle{ registry.get<ResourceHandle>(entity) };
		const entt::entity resourceEntity{ resourceHandle.getResourceEntity() };
		if (!registry.all_of<T>(resourceEntity)) {
			return { resourceEntity, nullptr };
		}

		return { resourceEntity, &registry.get<T>(resourceEntity) };
	}

	template <typename T>
	T* getResource(entt::registry& registry, const entt::entity entity) {
		return std::get<1>(getResourceAndEntity<T>(registry, entity));
	}

} // namespace Core
