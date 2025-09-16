module;

#include <utility>

#include <entt/entt.hpp>

module Core.Systems;

import Core.EnTTRegistry;
import Core.FileLoadSystem;
import Core.GLFWSystem;
import Core.LogReportingSystem;
import Core.ProcessErrorLoggingSystem;
import Core.ProcessErrorRetrySystem;
import Core.ResourceLoadSystem;
import Core.ReflectionContext;
import Core.Scheduler;
import Core.TypeLoaderSystem;

namespace Core {

	CoreSystems::CoreSystems(EnTTRegistry& registry, Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler{ scheduler }
		, mNodeParentGlobalSpatialUpdateSystem(registry, scheduler)
		, mGlobalSpatialPropagationSystem(registry, scheduler)
		, mLogReportingSystem{ registry, scheduler }
		, mProcessErrorLoggingSystem{ registry, scheduler }
		, mProcessErrorRetrySystem{ registry, scheduler }
		, mTypeLoaderSystem{ registry, scheduler }
		, mResourceLoadSystem{ registry, scheduler } {
		GLFWSystem::initSystem(mRegistry);
		FileLoadSystem::initSystem(mRegistry);

		mTickHandle = mScheduler.schedule([this] {
			GLFWSystem::tickSystem(mRegistry);
			FileLoadSystem::tickSystem(mRegistry);
		});
	}

	CoreSystems::~CoreSystems() {
		mScheduler.unschedule(std::move(mTickHandle));

		FileLoadSystem::destroySystem(mRegistry);
		GLFWSystem::destroySystem(mRegistry);

		// TODO: Required due to the static reflection context destruction order trying to access static TypeInfo
		getCurrentReflectionContext() = {};
	}

} // namespace Core
