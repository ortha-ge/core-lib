module;

#include <entt/fwd.hpp>

export module Core.ProcessErrorRetrySystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Core {

	class ProcessErrorRetrySystem {
	public:

		ProcessErrorRetrySystem(EnTTRegistry&, Scheduler&);
		~ProcessErrorRetrySystem();

		void tickSystem(entt::registry&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

	};

} // namespace Core
