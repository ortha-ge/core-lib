module;

#include <string>
#include <variant>

#include <entt/entt.hpp>

module Core.ProcessError;

namespace Core {

	void addProcessError(entt::registry& registry, const entt::entity entity, ProcessError::RetryTypes retry) {
		registry.emplace<ProcessError>(entity, std::nullopt, std::move(retry));
	}

	void addProcessError(entt::registry& registry, const entt::entity entity, ProcessError::RetryTypes retry, std::string errorMessage) {
		registry.emplace<ProcessError>(entity, std::move(errorMessage), std::move(retry));
	}

	bool hasHaltedProcessError(entt::registry& registry, const entt::entity entity) {
		if (!registry.all_of<ProcessError>(entity)) {
			return false;
		}

		const auto& processError = registry.get<ProcessError>(entity);
		if (!std::holds_alternative<bool>(processError.retry)) {
			return false;
		}

		return !std::get<bool>(processError.retry);
	}

} // namespace Core
