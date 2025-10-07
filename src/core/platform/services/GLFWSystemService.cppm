export module Ortha.Core.GLFWSystemService;

export import Ortha.Core.GLFWSystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Core {

	struct GLFWSystemService
		: kgr::single_service<GLFWSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
