module;

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

module Core.Node;

namespace Core {

	Node::Node() = default;

	Node::Node(std::string name)
		: mName(std::move(name)) {}

	void Node::destroy() {
		if (auto parentNode = getParentNode()) {
			parentNode->removeChild(shared_from_this());
		}

		for (auto&& child : mChildren) {
			child->destroy();
		}

		mChildren.clear();
	}

	TypeId Node::getTypeId() const {
		return TypeId::get<Node>();
	}

	void Node::addChild(Ptr child) {
		if (!child) {
			return;
		}

		auto parentNode = child->getParentNode();
		if (parentNode == shared_from_this()) {
			return;
		}

		if (parentNode) {
			parentNode->removeChild(child);
		}

		if (std::ranges::find(mChildren, child) != mChildren.end()) {
			return;
		}

		child->setParent(shared_from_this());
		mChildren.push_back(std::move(child));
	}

	void Node::removeChild(const Ptr& child) {
		if (!child) {
			return;
		}

		auto parentNode = child->getParentNode();
		if (parentNode != shared_from_this()) {
			return;
		}

		if (parentNode) {
			child->setParent({});
			;
		}

		auto it = std::ranges::find(mChildren, child);
		if (it != mChildren.end()) {
			mChildren.erase(it);
		}
	}

	void Node::setParent(Ptr parentNode) { mParent = parentNode; }

	const std::string& Node::getName() const { return mName; }

	Node::Ptr Node::getParentNode() const { return mParent.lock(); }

	const std::vector<Node::Ptr>& Node::getChildren() const { return mChildren; }

} // namespace Core
