export module Ortha.Core.LogReportingSystemService;

export import Ortha.Core.LogReportingSystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Core {

	struct LogReportingSystemService
		: kgr::single_service<LogReportingSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
