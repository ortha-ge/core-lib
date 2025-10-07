export module Ortha.Core.FileLoadSystemService;

export import Ortha.Core.FileLoadSystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Core {

	struct FileLoadSystemService
		: kgr::single_service<FileLoadSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
