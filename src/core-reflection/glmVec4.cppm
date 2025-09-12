module;

#include <glm/vec4.hpp>

export module Glm.Reflection.Vec4;

import Core.ReflectionContext;

template<>
void Core::reflect<glm::vec4>(ReflectionContext& reflectionContext) {
	reflectionContext.addClass<glm::vec4>("glm::vec4")
		.property("x", &glm::vec4::x)
		.property("y", &glm::vec4::y)
		.property("z", &glm::vec4::z)
		.property("w", &glm::vec4::w)
		.property("r", &glm::vec4::r)
		.property("g", &glm::vec4::g)
		.property("b", &glm::vec4::b)
		.property("a", &glm::vec4::a)
		.build();
}
