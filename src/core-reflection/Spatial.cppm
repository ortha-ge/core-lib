module;

#include <string_view>

export module Ortha.Core.Reflection.Spatial;

export import Ortha.Core.Spatial;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Reflect;
import Glm.Reflection.Vec3;

// TODO: Add inherits from
export namespace Ortha::Core {
	template <class SpatialType>
	void reflectSpatialType(RTTI::ReflectionContext& reflectionContext, std::string_view name) {
		reflectionContext.addClass<SpatialType>(name)
			.template field<&SpatialType::position>("position")
			// TODO: Properties that can convert from different types
			// TODO: read quaternion from euler angles in data
			//.property("rotation", &Spatial::rotation)
			.template field<&SpatialType::scale>("scale")
			//.annotate(createEnTTComponentAttribute<SpatialType>())
			;
	}
}

namespace Ortha::Core {

	template <>
	void reflect<Spatial>(RTTI::ReflectionContext& reflectionContext) {
		reflectSpatialType<Spatial>(reflectionContext, "Spatial");
	}

}
