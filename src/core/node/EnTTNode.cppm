module;

#include <string>

export module Core.EnTTNode;

import Core.Node;
import Core.TypeId;
import entt;

export namespace Core {

	class EnTTNode : public Node {
	public:
		EnTTNode();
		EnTTNode(std::string name, entt::handle entityHandle);
		~EnTTNode();

		void destroy() override;
		TypeId getTypeId() const override;

		entt::entity getEntity() const;

	private:
		entt::handle mEntityHandle;
	};

	const entt::entity createEnTTNode(entt::registry&, std::string);
	const entt::entity createChildEnTTNode(entt::registry&, entt::entity, std::string);
	const entt::entity createChildEnTTNode(entt::registry&, const Node::Ptr&, std::string);
	void addChildNode(entt::registry&, entt::entity, entt::entity);

} // namespace Core
