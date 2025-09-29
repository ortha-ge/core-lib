export module Core.GLFWSystemService;

export import Core.GLFWSystem;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;

export namespace Core {

	struct GLFWSystemService
		: kgr::single_service<GLFWSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
