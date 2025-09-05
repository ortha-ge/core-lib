
export module Core.SchedulerService;

import kangaru;

export import Core.Scheduler;

export namespace Core {

	class SchedulerService : public kgr::single_service<Scheduler> {};

} // namespace Core
