export module Ortha.Core.Reflection.Node;

export import Ortha.Core.Node;

import Ortha.Core.Reflect;

namespace Ortha::Core {

	template <>
	void reflect<Node>(RTTI::ReflectionContext& reflectionContext) {
		reflectionContext.addClass<Node>("Node")
			.field<&Node::mName>("name")
			.field<&Node::mChildren>("children")
			;
	}

}
