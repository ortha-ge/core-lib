module;

#include <entt/fwd.hpp>

export module Core.Systems;

import Core.EnTTRegistry;
import Core.LogReportingSystem;
import Core.ResourceLoadSystem;
import Core.Scheduler;
import Core.TypeLoaderSystem;

export namespace Core {

	class CoreSystems {
	public:
		CoreSystems(EnTTRegistry&, Scheduler&);
		~CoreSystems();

		CoreSystems(CoreSystems&&) = delete;
		CoreSystems& operator=(CoreSystems&&) noexcept = delete;

		CoreSystems(const CoreSystems&) = delete;
		CoreSystems& operator=(const CoreSystems&) = delete;

	private:
		entt::registry& mRegistry;
		Scheduler& mScheduler;
		LogReportingSystem mLogReportingSystem;
		TypeLoaderSystem mTypeLoaderSystem;
		ResourceLoadSystem mResourceLoadSystem;
		TaskHandle mTickHandle{};
	};

} // namespace Core
