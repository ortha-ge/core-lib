module;

#include <entt/entt.hpp>
#include <functional>
#include <memory>
#include <tuple>

export module Core.ResourceHandle;

import Core.Resource;

export namespace Core {

	class ResourceHandle {
	public:
		using TypeLoaderFunction = std::function<void(entt::registry&, entt::entity)>;

		ResourceHandle();
		explicit ResourceHandle(std::shared_ptr<Resource> resource);
		ResourceHandle(std::string resourceFilePath, TypeLoaderFunction typeLoaderFunc);
		~ResourceHandle();

		ResourceHandle(const ResourceHandle& other);
		ResourceHandle(ResourceHandle&& other) noexcept;

		ResourceHandle& operator=(const ResourceHandle& other);
		ResourceHandle& operator=(ResourceHandle&& other) noexcept;

		void setResource(std::shared_ptr<Resource> resource);
		[[nodiscard]] const std::string& getResourceFilePath() const;

		entt::entity createResource(entt::registry& registry) const;

		[[nodiscard]] entt::entity getResourceEntity() const;

		template<typename T, typename... Args>
		static std::shared_ptr<ResourceHandle> create(std::string resourceFilePath, Args&&... args);

	private:

		std::string mResourceFilePath;
		TypeLoaderFunction mTypeLoaderFunc;
		std::shared_ptr<Resource> mResource;

	};

	template<typename T, typename... Args>
	std::shared_ptr<ResourceHandle> ResourceHandle::create(std::string resourceFilePath, Args&&... args) {
		auto argTuple = std::make_tuple(std::forward<Args>(args)...);
		auto typeLoader = [args = std::move(argTuple)](entt::registry& registry, entt::entity entity) {
			T instance{ std::make_from_tuple<T>(std::move(args)) };
			registry.emplace<T>(entity, std::move(instance));
		};

		return std::make_shared<ResourceHandle>(std::move(resourceFilePath), std::move(typeLoader));
	}

	// OLD
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
