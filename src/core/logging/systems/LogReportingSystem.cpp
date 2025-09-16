module;

#include <cstdio>
#include <utility>

#include <entt/entt.hpp>
#include <tracy/Tracy.hpp>

module Core.LogReportingSystem;

import Core.Log;
import Core.LogReportedFlag;

namespace Core {


	LogReportingSystem::LogReportingSystem(EnTTRegistry& registry, Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() {
			tickSystem(mRegistry);
		});
	}

	LogReportingSystem::~LogReportingSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void LogReportingSystem::tickSystem(entt::registry& registry) {
		ZoneScoped;

		registry.view<Log>(entt::exclude<LogReportedFlag>)
			.each([&registry](const entt::entity entity, Log& log) {
				for (auto&& message : log.messages) {
					if (!message.logged) {
						printf("Log: %s\n", message.message.c_str());
						message.logged = true;
					}
				}

				if (log.removeEntityWhenReported) {
					registry.destroy(entity);
				} else if (log.clearMessagesWhenReported) {
					registry.remove<Log>(entity);
				}

				registry.emplace<LogReportedFlag>(entity);
			});
	}

} // namespace Core
