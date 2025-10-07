export module Ortha.Core.SchedulerService;

export import Ortha.Core.Scheduler;

import kangaru;

export namespace Ortha::Core {

	class SchedulerService : public kgr::single_service<Scheduler> {};

} // namespace Ortha::Core
