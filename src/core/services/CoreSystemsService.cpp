module Core.SystemsService;

import Core.EntityVisitorSystemService;
import Core.FileLoadSystemService;
import Core.GLFWSystemService;
import Core.GlobalSpatialUpdateSystemService;
import Core.LogReportingSystemService;
import Core.NodeParentGlobalSpatialUpdateSystemService;
import Core.ProcessErrorLoggingSystemService;
import Core.ProcessErrorRetrySystemService;
import Core.Reflection;
import Core.ReflectionContext;
import Core.ResourceLoadSystemService;
import Core.TypeLoaderSystemService;
import kangaru;

namespace Core {

	void initCoreSubSystems(CoreSystems&, kgr::container& container) {
		reflectAll(getCurrentReflectionContext());

		// Platform
		container.emplace<GLFWSystemService>();

		// Visitor
		container.emplace<EntityVisitorSystemService>();

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
