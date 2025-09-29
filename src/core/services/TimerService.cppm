export module Core.TimerService;

export import Core.Timer;

import Core.SchedulerService;
import kangaru;

export namespace Core {

	class TimerService : public kgr::single_service<Timer, kgr::dependency<SchedulerService>> {};

} // namespace Core
