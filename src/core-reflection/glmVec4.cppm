export module Glm.Reflection.Vec4;

export import glm;

import Core.Reflect;

template<>
void Core::reflect<glm::vec4>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	reflectionContext.addClass<glm::vec4>("glm::vec4")
		.field<&glm::vec4::x>("x")
		.field<&glm::vec4::y>("y")
		.field<&glm::vec4::z>("z")
		.field<&glm::vec4::w>("w")
		.field<&glm::vec4::r>("r")
		.field<&glm::vec4::g>("g")
		.field<&glm::vec4::b>("b")
		.field<&glm::vec4::a>("a")
		;
}
