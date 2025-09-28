module;

#include <tracy/Tracy.hpp>

module Core.ProcessErrorLoggingSystem;

import Core.Log;
import Core.ProcessError;
import Core.ProcessErrorLoggedFlag;
import entt;

namespace Core {

	ProcessErrorLoggingSystem::ProcessErrorLoggingSystem(entt::registry& registry, Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() {
			tickSystem(mRegistry);
		});
	}

	ProcessErrorLoggingSystem::~ProcessErrorLoggingSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void ProcessErrorLoggingSystem::tickProcessError(entt::registry& registry, const entt::entity entity, const ProcessError& processError) {
		if (processError.errorMessage) {
			logEntry(registry, entity, *processError.errorMessage);
		}
		registry.emplace<ProcessErrorLoggedFlag>(entity);
	}

	void ProcessErrorLoggingSystem::tickSystem(entt::registry& registry) {
		ZoneScopedN("ProcessErrorLoggingSystem::tickSystem");

		registry.view<ProcessError>(entt::exclude<ProcessErrorLoggedFlag>)
			.each([&registry](const entt::entity entity, const ProcessError& processError) {
				tickProcessError(registry, entity, processError);
			});
	}

} // namespace Core
