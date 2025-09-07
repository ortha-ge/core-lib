module;

#include <string>
#include <utility>

#include <entt/entt.hpp>

module Core.Log;

import Core.LogReportedFlag;

namespace Core {

	void logEntry(Log& log, std::string message) {
		log.messages.emplace_back(std::move(message));
	}

	void logEntry(entt::registry& registry, std::string message) {
		const entt::entity logEntity{ registry.create() };
		logEntry(registry, logEntity, std::move(message));
	}

	void logEntry(entt::registry& registry, const entt::entity entity, std::string message) {
		auto& log{ registry.get_or_emplace<Log>(entity) };
		logEntry(log, std::move(message));
		registry.remove<LogReportedFlag>(entity);
	}

	void logEntries(entt::registry& registry, Log log) {
		const entt::entity logEntity{ registry.create() };
		logEntries(registry, logEntity, std::move(log));
	}

	void logEntries(entt::registry& registry, entt::entity entity, Log log) {
		auto& newOrExistingLog{ registry.get_or_emplace<Log>(entity) };
		appendLogEntries(newOrExistingLog, std::move(log));
		registry.remove<LogReportedFlag>(entity);
	}

	void appendLogEntries(Log& destLog, Log srcLog) {
		destLog.messages.insert(destLog.messages.end(),
			std::make_move_iterator(srcLog.messages.begin()),
			std::make_move_iterator(srcLog.messages.end())
		);
	}

} // namespace Core
