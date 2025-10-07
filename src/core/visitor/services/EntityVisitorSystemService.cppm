
export module Ortha.Core.EntityVisitorSystemService;

export import Ortha.Core.EntityVisitorSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Core {

	struct EntityVisitorSystemService
		: kgr::single_service<EntityVisitorSystem, kgr::dependency<EnTTRegistryService, SchedulerService>> {};

} // namespace Ortha::Core
