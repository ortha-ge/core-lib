export module Ortha.Core.Reflection.GlobalSpatial;

export import Ortha.Core.GlobalSpatial;

import Ortha.Core.Reflection.Spatial;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template <>
	void reflect<Core::GlobalSpatial>(ReflectionContext& reflectionContext) {
		using namespace Core;

		reflectionContext.addClass<GlobalSpatial>("GlobalSpatial")
			.base<SpatialData>();
	}

}