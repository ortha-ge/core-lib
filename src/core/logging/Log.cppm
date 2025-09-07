module;

#include <format>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <entt/entt.hpp>

export module Core.Log;

export namespace Core {

	struct LogMessage {
		std::string message;
		bool logged{ false };
	};

	struct Log {
		std::vector<LogMessage> messages;
		bool clearMessagesWhenReported{ false };
		bool removeEntityWhenReported{ false };
	};

	void logEntry(Log&, std::string);
	void logEntry(entt::registry&, std::string);
	void logEntry(entt::registry&, entt::entity, std::string);
	void logEntries(entt::registry&, Log);
	void logEntries(entt::registry&, entt::entity, Log);
	void appendLogEntries(Log&, Log);

	template <typename ... Args>
	void logEntry(Log& log, std::string_view formatString, Args&& ... args) {
		std::string formattedString = std::vformat(formatString, std::make_format_args(args...));
		logEntry(log, std::move(formattedString));
	}

	template <typename ... Args>
	void logEntry(entt::registry& registry, std::string_view formatString, Args&& ... args) {
		std::string formattedString = std::vformat(formatString, std::make_format_args(args...));
		logEntry(registry, std::move(formattedString));
	}

	template <typename ... Args>
	void logEntry(entt::registry& registry, const entt::entity entity, std::string_view formatString, Args&& ... args) {
		std::string formattedString = std::vformat(formatString, std::make_format_args(args...));
		logEntry(registry, entity, std::move(formattedString));
	}
} // namespace Core
