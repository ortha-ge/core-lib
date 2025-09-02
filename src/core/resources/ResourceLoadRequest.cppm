module;

#include <functional>
#include <string>
#include <utility>

#include <entt/entt.hpp>

export module Core.ResourceLoadRequest;

export namespace Core {

	class ResourceLoadRequest {
	public:

		template <typename T, typename ... Args>
		static ResourceLoadRequest create(std::string resourceFilePath, Args&& ... args);

		const std::string& getResourceFilePath() const;
		entt::entity createResource(entt::registry& registry) const;

	private:

		using TypeLoaderFunction = std::function<void(entt::registry&, entt::entity)>;

		ResourceLoadRequest(std::string resourceFilePath, TypeLoaderFunction typeLoaderFunction);

		std::string resourceFilePath{};
		TypeLoaderFunction typeLoaderFunction;
	};

	template <typename T, typename ... Args>
	ResourceLoadRequest ResourceLoadRequest::create(std::string resourceFilePath, Args&& ... args) {
		auto typeLoader = [...args = std::move(args)](entt::registry& registry, entt::entity entity) {
			registry.emplace<T>(entity, std::move(args)...);
		};
		ResourceLoadRequest loadRequest(std::move(resourceFilePath), std::move(typeLoader));
		return loadRequest;
	}

} // namespace Core
