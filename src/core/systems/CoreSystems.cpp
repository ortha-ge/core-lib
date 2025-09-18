
module Core.Systems;

import Core.ReflectionContext;

namespace Core {

	CoreSystems::CoreSystems() = default;

	CoreSystems::~CoreSystems() {
		// TODO: Required due to the static reflection context destruction order trying to access static TypeInfo
		getCurrentReflectionContext() = {};
	}

} // namespace Core
