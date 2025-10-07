export module Ortha.Core.ResourceLoadSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::Core {

	class ResourceLoadSystem {
	public:
		ResourceLoadSystem(entt::registry&, Scheduler&);
		~ResourceLoadSystem();

		void tick(entt::registry&);

	private:
		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

		entt::entity mResourceCacheEntity{ entt::null };
	};

} // namespace Ortha::Core
