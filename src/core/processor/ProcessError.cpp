module;

#include <string>

#include <entt/entt.hpp>

module Core.ProcessError;

namespace Core {

	void addProcessError(entt::registry& registry, const entt::entity entity, ProcessError::RetryTypes retry, std::string errorMessage) {
		registry.emplace<ProcessError>(entity, std::move(errorMessage), std::move(retry));
	}

} // namespace Core
