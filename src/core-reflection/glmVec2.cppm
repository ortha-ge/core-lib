export module Glm.Reflection.Vec2;

export import glm;

import Ortha.Core.Reflect;

template<>
void Core::reflect<glm::vec2>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	reflectionContext.addClass<glm::vec2>("glm::vec2")
		.field<&glm::vec2::x>("x")
		.field<&glm::vec2::y>("y")
		.field<&glm::vec2::x>("u")
		.field<&glm::vec2::y>("v")
		;
}

template<>
void Core::reflect<glm::ivec2>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	reflectionContext.addClass<glm::ivec2>("glm::ivec2")
		.field<&glm::ivec2::x>("x")
		.field<&glm::ivec2::y>("y")
		.field<&glm::ivec2::x>("u")
		.field<&glm::ivec2::y>("v")
		;
}
