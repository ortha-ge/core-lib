module;

#include <glm/vec2.hpp>

export module Glm.Reflection.Vec2;

import Core.ReflectionContext;

template<>
void Core::reflect<glm::vec2>(ReflectionContext& reflectionContext) {
	reflectionContext.addClass<glm::vec2>("glm::vec2")
		.property("x", &glm::vec2::x)
		.property("y", &glm::vec2::y)
		.property("u", &glm::vec2::x)
		.property("v", &glm::vec2::y)
		.build();
}
