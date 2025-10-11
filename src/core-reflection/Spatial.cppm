module;

#include <string_view>

export module Ortha.Core.Reflection.Spatial;

export import Ortha.Core.Spatial;

import Glm.Reflection.Vec3;
import Ortha.Core.EnTTComponentAttribute;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template <>
	void reflect<Core::SpatialData>(ReflectionContext& reflectionContext) {
		using namespace Core;
		reflectionContext.addClass<SpatialData>("SpatialData")
			.field<&Spatial::position>("position")
			// TODO: Properties that can convert from different types
			// TODO: read quaternion from euler angles in data
			//.property("rotation", &Spatial::rotation)
			.field<&Spatial::scale>("scale")
			//.annotate(createEnTTComponentAttribute<SpatialType>())
			;
	}

	template <>
	void reflect<Core::Spatial>(ReflectionContext& reflectionContext) {
		using namespace Core;

		reflectionContext.addClass<Spatial>("Spatial")
			.base<SpatialData>();
	}

}
