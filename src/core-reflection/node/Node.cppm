export module Ortha.Core.Reflection.Node;

export import Ortha.Core.Node;

import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template <>
	void reflect<Core::Node>(ReflectionContext& reflectionContext) {
		using namespace Core;
		reflectionContext.addClass<Node>("Node")
			.field<&Node::mName>("name")
			.field<&Node::mChildren>("children");
	}

}
