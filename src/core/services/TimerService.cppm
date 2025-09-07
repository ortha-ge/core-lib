
export module Core.TimerService;

export import Core.Timer;

import kangaru;
import Core.SchedulerService;

export namespace Core {

	class TimerService : public kgr::single_service<Timer, kgr::dependency<SchedulerService>> {};

} // namespace Core
