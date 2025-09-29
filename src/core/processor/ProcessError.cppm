module;

#include <chrono>
#include <cstdint>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

export module Core.ProcessError;

import entt;

export namespace Core {

	struct ProcessError {
		using RetryTimepoint = std::chrono::steady_clock::time_point;
		using RetryTypes = std::variant<uint32_t, RetryTimepoint, bool>;

		std::optional<std::string> errorMessage{};
		RetryTypes retry;
	};

	void addProcessError(entt::registry&, entt::entity, ProcessError::RetryTypes);
	void addProcessError(entt::registry&, entt::entity, ProcessError::RetryTypes, std::string);
	bool hasHaltedProcessError(entt::registry&, entt::entity);

	template <typename ... Args>
	void addProcessError(entt::registry& registry, entt::entity entity, std::string_view formatMessage, Args&&... args) {
		std::string formattedString = std::vformat(formatMessage, std::make_format_args(args...));
		addProcessError(registry, entity, false, std::move(formattedString));
	}

	template <typename ... Args>
	void addProcessError(entt::registry& registry, entt::entity entity, ProcessError::RetryTypes retry, std::string_view formatMessage, Args&&... args) {
		std::string formattedString = std::vformat(formatMessage, std::make_format_args(args...));
		addProcessError(registry, entity, retry, std::move(formattedString));
	}

} // namespace Core
