module;

#include <string>

#include <entt/entt.hpp>

export module Core.EnTTNode;

import Core.Node;
import Core.TypeId;

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

} // namespace Core
