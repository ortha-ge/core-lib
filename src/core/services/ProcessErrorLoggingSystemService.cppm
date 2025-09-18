
export module Core.ProcessErrorLoggingSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export import Core.ProcessErrorLoggingSystem;

export namespace Core {

	struct ProcessErrorLoggingSystemService
		: kgr::single_service<ProcessErrorLoggingSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
