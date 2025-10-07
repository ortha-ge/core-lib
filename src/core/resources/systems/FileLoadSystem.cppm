export module Ortha.Core.FileLoadSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Core {

	class FileLoadSystem {
	public:

		FileLoadSystem(entt::registry&, Scheduler&);
		~FileLoadSystem();

		static void tickSystem(entt::registry& registry);

	private:

		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};

} // namespace Ortha::Core
