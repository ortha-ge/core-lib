module;

#include <string_view>

export module Core.Reflection.Spatial;

export import Core.Spatial;

import Core.EnTTClassAnnotation;
import Core.ReflectionContext;
import Glm.Reflection.Vec3;

// TODO: Add inherits from
export namespace Core {
	template <class SpatialType>
	void reflectSpatialType(ReflectionContext& reflectionContext, std::string_view name) {
		reflectionContext.addClass<SpatialType>(name)
			.property("position", &SpatialType::position)
			//.property("rotation", &Spatial::rotation)
			.property("scale", &SpatialType::scale)
			.annotate(createEnTTClassAnnotation<SpatialType>())
			.build();
	}
}

template <>
void Core::reflect<Core::Spatial>(ReflectionContext& reflectionContext) {
	reflectSpatialType<Spatial>(reflectionContext, "Spatial");
}