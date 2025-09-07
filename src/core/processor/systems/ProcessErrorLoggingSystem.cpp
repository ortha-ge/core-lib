module;

#include <entt/entt.hpp>

module Core.ProcessErrorLoggingSystem;

import Core.Log;
import Core.ProcessError;
import Core.ProcessErrorLoggedFlag;

namespace Core {

	ProcessErrorLoggingSystem::ProcessErrorLoggingSystem(EnTTRegistry& registry, Scheduler& scheduler)
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
		logEntry(registry, entity, processError.errorMessage);
		registry.emplace<ProcessErrorLoggedFlag>(entity);
	}

	void ProcessErrorLoggingSystem::tickSystem(entt::registry& registry) {
		registry.view<ProcessError>(entt::exclude<ProcessErrorLoggedFlag>)
			.each([&registry](const entt::entity entity, const ProcessError& processError) {
				tickProcessError(registry, entity, processError);
			});
	}

} // namespace Core
