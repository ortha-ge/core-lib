export module Core.ProcessErrorLoggingSystemService;

export import Core.ProcessErrorLoggingSystem;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;

export namespace Core {

	struct ProcessErrorLoggingSystemService
		: kgr::single_service<ProcessErrorLoggingSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
