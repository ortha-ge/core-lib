module;

#include <memory>
#include <string_view>

#include <entt/fwd.hpp>

export module Core.TypeLoader;

export namespace Core {

    class TypeLoaderAdapter {
    public:

        virtual ~TypeLoaderAdapter() = default;

        virtual void load(std::string_view, entt::registry&, entt::entity) = 0;

    };

    struct TypeLoader {
        std::shared_ptr<TypeLoaderAdapter> adapter;
    };

} // Core
