module;

#include <utility>

#include <entt/entt.hpp>

module Core.Systems;

import Core.EnTTRegistry;
import Core.FileLoadSystem;
import Core.GLFWSystem;
import Core.Scheduler;
import Core.TypeLoaderSystem;

namespace Core {

	CoreSystems::CoreSystems(EnTTRegistry& registry, Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler{ scheduler }
		, mTypeLoaderSystem{ registry, scheduler } {
		GLFWSystem::initSystem(mRegistry);
		FileLoadSystem::initSystem(mRegistry);

		mTickHandle = mScheduler.schedule([this]{
			GLFWSystem::tickSystem(mRegistry);
			FileLoadSystem::tickSystem(mRegistry);
		});
	}

	CoreSystems::~CoreSystems() {
		mScheduler.unschedule(std::move(mTickHandle));

		FileLoadSystem::destroySystem(mRegistry);
		GLFWSystem::destroySystem(mRegistry);
	}
	
} // Core
