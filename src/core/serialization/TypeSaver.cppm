module;

#include <memory>
#include <string_view>

#include <entt/fwd.hpp>

export module Core.TypeSaver;

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
