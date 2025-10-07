export module Ortha.Core.ProcessErrorLoggingSystemService;

export import Ortha.Core.ProcessErrorLoggingSystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Ortha::Core {

	struct ProcessErrorLoggingSystemService
		: kgr::single_service<ProcessErrorLoggingSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Ortha::Core
