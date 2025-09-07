module;

#include <entt/entt.hpp>

module Core.ProcessErrorLoggingSystem;

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

	void ProcessErrorLoggingSystem::tickSystem(entt::registry& registry) {

	}

} // namespace Core
