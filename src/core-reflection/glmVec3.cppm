export module Glm.Reflection.Vec3;

import Core.ReflectionContext;
import glm;

template<>
void Core::reflect<glm::vec3>(ReflectionContext& reflectionContext) {
	reflectionContext.addClass<glm::vec3>("glm::vec3")
		.property("x", &glm::vec3::x)
		.property("y", &glm::vec3::y)
		.property("z", &glm::vec3::z)
		.property("r", &glm::vec3::r)
		.property("g", &glm::vec3::g)
		.property("b", &glm::vec3::b)
		.build();
}
