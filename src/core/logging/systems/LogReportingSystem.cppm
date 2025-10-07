export module Ortha.Core.LogReportingSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Core {

	class LogReportingSystem {
	public:

		LogReportingSystem(entt::registry&, Scheduler&);
		~LogReportingSystem();

		static void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

	};

} // namespace Core
