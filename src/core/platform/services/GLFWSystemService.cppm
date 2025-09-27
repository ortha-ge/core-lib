
export module Core.GLFWSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export import Core.GLFWSystem;

export namespace Core {

	struct GLFWSystemService
		: kgr::single_service<GLFWSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
