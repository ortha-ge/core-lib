module;

#include <entt/entt.hpp>

module Core.ProcessErrorRetrySystem;

namespace Core {

	ProcessErrorRetrySystem::ProcessErrorRetrySystem(EnTTRegistry& registry, Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() {
			tickSystem(mRegistry);
		});
	}

	ProcessErrorRetrySystem::~ProcessErrorRetrySystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void ProcessErrorRetrySystem::tickSystem(entt::registry& registry) {

	}

} // namespace Core
