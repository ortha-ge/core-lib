
export module Core.ProcessErrorRetrySystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export import Core.ProcessErrorRetrySystem;

export namespace Core {

	struct ProcessErrorRetrySystemService
		: kgr::single_service<ProcessErrorRetrySystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
