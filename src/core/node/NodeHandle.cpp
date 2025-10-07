module;

#include <functional>
#include <memory>
#include <string>

module Ortha.Core.NodeHandle;

namespace Core {

	NodeHandle NodeHandle::create(std::string name) { return NodeHandle(std::make_shared<Node>(name)); }

	const Node::Ptr& NodeHandle::getNode() const { return mNode; }

	Node* NodeHandle::operator->() {
		return mNode.get();
	}
	const Node* NodeHandle::operator->() const {
		return mNode.get();
	}

	NodeHandle::NodeHandle(Node::Ptr node)
		: mNode(std::move(node)) {}

	Node::Ptr getRootNode(const NodeHandle& node) {
		Node::Ptr rootNode = node.getNode();
		while (rootNode) {
			Node::Ptr parentNode = rootNode->getParentNode();
			if (!parentNode) {
				break;
			}

			rootNode = parentNode;
		}

		return rootNode;
	}

	void _visitParentHeirarchy(Node::Ptr node, const std::function<void(const Node::Ptr&, const Node::Ptr&)>& visitor) {
		if (!node) {
			return;
		}

		Node::Ptr parentNode = node->getParentNode();
		_visitParentHeirarchy(parentNode, visitor);
		visitor(parentNode, node);
	}

	void visitParentHeirarchy(const NodeHandle& node, const std::function<void(const Node::Ptr&, const Node::Ptr&)>& visitor) {
		_visitParentHeirarchy(node.getNode(), visitor);
	}

} // namespace Core
