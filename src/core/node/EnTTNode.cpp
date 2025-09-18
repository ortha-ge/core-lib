module;

#include <entt/entt.hpp>

module Core.EnTTNode;

namespace Core {

	EnTTNode::EnTTNode() = default;

	EnTTNode::EnTTNode(std::string name, entt::handle entityHandle)
		: Node(std::move(name))
		, mEntityHandle(entityHandle) {}

	EnTTNode::~EnTTNode() = default;

	void EnTTNode::destroy() {
		Node::destroy();

		if (mEntityHandle.valid()) {
			mEntityHandle.destroy();
		}
	}

	TypeId EnTTNode::getTypeId() const {
		return TypeId::get<EnTTNode>();
	}

	entt::entity EnTTNode::getEntity() const {
		return mEntityHandle.entity();
	}

	void addChildNode(entt::registry& registry, const entt::entity parent, const entt::entity child) {
		const auto* parentNodeHandle = registry.try_get<NodeHandle>(parent);
		const auto* childNodeHandle = registry.try_get<NodeHandle>(child);
		if (parentNodeHandle && childNodeHandle) {
			parentNodeHandle->getNode()->addChild(childNodeHandle->getNode());
		}
	}

} // namespace Core
