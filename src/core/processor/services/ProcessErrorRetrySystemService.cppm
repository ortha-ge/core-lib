export module Ortha.Core.ProcessErrorRetrySystemService;

export import Ortha.Core.ProcessErrorRetrySystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;


export namespace Core {

	struct ProcessErrorRetrySystemService
		: kgr::single_service<ProcessErrorRetrySystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
