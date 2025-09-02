module;

#include <string>
#include <utility>

#include <entt/entt.hpp>

module Core.ResourceLoadRequest;

namespace Core {

	ResourceLoadRequest::ResourceLoadRequest(std::string resourceFilePath, TypeLoaderFunction typeLoaderFunction)
		: resourceFilePath(std::move(resourceFilePath))
		, typeLoaderFunction(std::move(typeLoaderFunction)) {
	}

	const std::string& ResourceLoadRequest::getResourceFilePath() const {
		return resourceFilePath;
	}

	entt::entity ResourceLoadRequest::createResource(entt::registry& registry) const {
		entt::entity resourceEntity{ registry.create() };
		typeLoaderFunction(registry, resourceEntity);
		return resourceEntity;
	}

} // namespace Core
