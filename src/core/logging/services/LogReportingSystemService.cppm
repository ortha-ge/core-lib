
export module Core.LogReportingSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export import Core.LogReportingSystem;

export namespace Core {

	struct LogReportingSystemService
		: kgr::single_service<LogReportingSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
