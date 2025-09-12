module;

#include <string>

#include <entt/entt.hpp>

export module Core.EnTTNode;

import Core.Node;

export namespace Core {

	class EnTTNode : public Node {
	public:
		EnTTNode(std::string name, entt::handle entityHandle);
		~EnTTNode();

		void destroy() override;

	private:
		entt::handle mEntityHandle;
	};

} // namespace Core
