
export module Core.EntityVisitorSystemService;

export import Core.EntityVisitorSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Core {

	struct EntityVisitorSystemService
		: kgr::single_service<EntityVisitorSystem, kgr::dependency<EnTTRegistryService, SchedulerService>> {};

} // namespace Core
