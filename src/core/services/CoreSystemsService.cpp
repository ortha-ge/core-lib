module;

#include <entt/entt.hpp>

module Core.SystemsService;

import kangaru;
import Core.FileLoadSystemService;
import Core.GLFWSystemService;
import Core.GlobalSpatialUpdateSystemService;
import Core.LogReportingSystemService;
import Core.NodeParentGlobalSpatialUpdateSystemService;
import Core.ProcessErrorLoggingSystemService;
import Core.ProcessErrorRetrySystemService;
import Core.ResourceLoadSystemService;
import Core.TypeLoaderSystemService;

namespace Core {

	void initCoreSubSystems(CoreSystems&, kgr::container& container) {
		// Platform
		container.emplace<GLFWSystemService>();

		// GlobalSpatial
		container.emplace<NodeParentGlobalSpatialUpdateSystemService>();
		container.emplace<GlobalSpatialUpdateSystemService>();

		// Resource
		container.emplace<FileLoadSystemService>();
		container.emplace<ResourceLoadSystemService>();
		container.emplace<TypeLoaderSystemService>();

		// ProcessError
		container.emplace<ProcessErrorRetrySystemService>();
		container.emplace<ProcessErrorLoggingSystemService>();

		// Log
		container.emplace<LogReportingSystemService>();
	}

} // namespace Core
