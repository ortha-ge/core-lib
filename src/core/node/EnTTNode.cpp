module;

#include <entt/entt.hpp>

module Core.EnTTNode;

namespace Core {

	EnTTNode::EnTTNode(std::string name, entt::handle entityHandle)
		: Node(std::move(name))
		, mEntityHandle(std::move(entityHandle)) {}

	EnTTNode::~EnTTNode() {}

	void EnTTNode::destroy() {
		Node::destroy();

		if (mEntityHandle.valid()) {
			mEntityHandle.destroy();
		}
	}

} // namespace Core
