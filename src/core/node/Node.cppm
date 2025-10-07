module;

#include <list>
#include <memory>
#include <string>

export module Core.Node;

import Ortha.RTTI.TypeId;

export namespace Core {

	class Node : public std::enable_shared_from_this<Node> {
	public:
		using Ptr = std::shared_ptr<Node>;
		using WeakPtr = std::weak_ptr<Node>;

		Node();
		explicit Node(std::string name);
		virtual ~Node() = default;

		virtual void destroy();
		virtual Ortha::RTTI::TypeId getTypeId() const;

		void addChild(Ptr child);
		void removeChild(const Ptr& child);

		void setName(std::string name);
		void setParent(Ptr parentNode);

		const std::string& getName() const;
		Ptr getParentNode() const;
		const std::list<Ptr>& getChildren() const;

	//private:
		std::string mName;
		WeakPtr mParent;
		std::list<Ptr> mChildren;
	};

} // namespace Core
