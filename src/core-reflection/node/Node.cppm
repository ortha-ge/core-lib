export module Core.Reflection.Node;

export import Core.Node;

import Core.ReflectionContext;

template <>
void Core::reflect<Core::Node>(ReflectionContext& reflectionContext) {
	reflectionContext.addClass<Node>("Node")
		.property("name", &Node::mName)
		//TODO: How to auto-reflect recursive things.
		.propertyNoAutoReflect("children", &Node::mChildren)
		.build();
}
