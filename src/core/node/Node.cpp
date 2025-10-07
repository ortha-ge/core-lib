module;

#include <algorithm>
#include <list>
#include <memory>
#include <string>

module Ortha.Core.Node;

import Ortha.RTTI.TypeHandle;

namespace Core {

	Node::Node() = default;

	Node::Node(std::string name)
		: mName(std::move(name)) {}

	void Node::destroy() {
		if (auto parentNode = getParentNode()) {
			parentNode->removeChild(shared_from_this());
		}


		for (auto it = mChildren.begin(); it != mChildren.end();) {
			auto currentIt = it++;
			(*currentIt)->destroy();
		}

		mChildren.clear();
	}

	Ortha::RTTI::TypeId Node::getTypeId() const {
		return Ortha::RTTI::TypeId(Ortha::RTTI::TypeHandle::get<Node>());
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

	void Node::setName(std::string name) {
		mName = std::move(name);
	}

	void Node::setParent(Ptr parentNode) { mParent = parentNode; }

	const std::string& Node::getName() const { return mName; }

	Node::Ptr Node::getParentNode() const { return mParent.lock(); }

	const std::list<Node::Ptr>& Node::getChildren() const { return mChildren; }

} // namespace Core
