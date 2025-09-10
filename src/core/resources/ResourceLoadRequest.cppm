module;

#include <functional>
#include <memory>
#include <string>

#include <entt/entt.hpp>

export module Core.ResourceLoadRequest;

import Core.ResourceHandle;

export namespace Core {

	class ResourceLoadRequest {
	public:
		using TypeLoaderFunction = std::function<void(entt::registry&, entt::entity)>;

		ResourceLoadRequest(std::shared_ptr<ResourceHandle>, TypeLoaderFunction typeLoaderFunc);

		[[nodiscard]] std::shared_ptr<ResourceHandle> lockResourceHandle() const;
		entt::entity createResource(entt::registry& registry) const;

		template<typename T, typename... Args>
		static std::shared_ptr<ResourceHandle> create(entt::registry&, std::string, Args&&...);

	private:

		std::weak_ptr<ResourceHandle> mResourceHandle;
		TypeLoaderFunction mTypeLoaderFunc;

	};

	template<typename T, typename... Args>
	std::shared_ptr<ResourceHandle> ResourceLoadRequest::create(entt::registry& registry, std::string resourceFilePath, Args&&... args) {

		auto argTuple = std::make_tuple(std::forward<Args>(args)...);
		auto typeLoader = [args = std::move(argTuple)](entt::registry& registry, entt::entity entity) {
			T instance{ std::make_from_tuple<T>(std::move(args)) };
			registry.emplace<T>(entity, std::move(instance));
		};

		auto resourceHandle = std::make_shared<ResourceHandle>(std::move(resourceFilePath));

		auto loadRequest = registry.create();
		registry.emplace<ResourceLoadRequest>(loadRequest, resourceHandle, std::move(typeLoader));

		return resourceHandle;
	}

} // namespace Core
