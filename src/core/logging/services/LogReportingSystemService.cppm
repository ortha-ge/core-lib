export module Core.LogReportingSystemService;

export import Core.LogReportingSystem;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;

export namespace Core {

	struct LogReportingSystemService
		: kgr::single_service<LogReportingSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
