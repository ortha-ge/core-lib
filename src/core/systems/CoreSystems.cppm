module;

#include <entt/fwd.hpp>

export module Core.Systems;

import Core.EnTTRegistry;
import Core.Scheduler;
import Core.TypeLoaderSystem;
import Core.ResourceLoadSystem;

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
		TypeLoaderSystem mTypeLoaderSystem;
		ResourceLoadSystem mResourceLoadSystem;
		TaskHandle mTickHandle{};

	};

} // Core