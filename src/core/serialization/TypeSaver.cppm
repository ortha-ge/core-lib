module;

#include <memory>
#include <string_view>

export module Core.TypeSaver;

import entt;

export namespace Core {

	class TypeSaverAdapter {
	public:
		virtual ~TypeSaverAdapter() = default;

		virtual void save(std::string_view, entt::registry&, entt::entity) = 0;
	};

	struct TypeSaver {
		std::unique_ptr<TypeSaverAdapter> adapter;
	};

} // namespace Core
