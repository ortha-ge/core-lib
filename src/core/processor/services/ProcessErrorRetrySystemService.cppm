export module Core.ProcessErrorRetrySystemService;

export import Core.ProcessErrorRetrySystem;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;


export namespace Core {

	struct ProcessErrorRetrySystemService
		: kgr::single_service<ProcessErrorRetrySystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
