export module Ortha.Core.TimerService;

export import Ortha.Core.Timer;

import Ortha.Core.SchedulerService;
import kangaru;

export namespace Core {

	class TimerService : public kgr::single_service<Timer, kgr::dependency<SchedulerService>> {};

} // namespace Core
