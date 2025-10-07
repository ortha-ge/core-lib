export module Ortha.Core.TypeLoaderSystem;

import Ortha.Core.Scheduler;

import entt;

export namespace Core {

	class TypeLoaderSystem {
	public:
		TypeLoaderSystem(entt::registry&, Scheduler&);
		~TypeLoaderSystem();

		void tickSystem(entt::registry&);

	private:
		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;
	};

} // namespace Core
