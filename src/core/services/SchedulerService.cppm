export module Core.SchedulerService;

export import Core.Scheduler;

import kangaru;

export namespace Core {

	class SchedulerService : public kgr::single_service<Scheduler> {};

} // namespace Core
