module;

#include <entt/fwd.hpp>

export module Core.FileLoadSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Core {

	class FileLoadSystem {
	public:

		FileLoadSystem(EnTTRegistry&, Scheduler&);
		~FileLoadSystem();

		static void tickSystem(entt::registry& registry);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};

} // namespace Core
