export module Core.Reflection.Node;

export import Core.Node;

import Core.Reflect;

template <>
void Core::reflect<Core::Node>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	reflectionContext.addClass<Node>("Node")
		.field<&Node::mName>("name")
		//TODO: How to auto-reflect recursive things.
		.field<&Node::mChildren>("children")
		;
}
