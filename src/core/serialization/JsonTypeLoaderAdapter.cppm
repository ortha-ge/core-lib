module;

#include <string_view>

#include <entt/fwd.hpp>

export module Core.JsonTypeLoaderAdapter;

import Core.Any;
import Core.TypeLoader;

export namespace Core {

    void load(std::string_view jsonInput, Any& anyValue);

    template <typename T>
    class JsonTypeLoaderAdapter : public TypeLoaderAdapter {
    public:

        T _load(std::string_view jsonInput) {
            T instance{};
            Any any(instance);
            Core::load(jsonInput, any);
            return instance;
        }

        template <typename Registry>
        void _load(std::string_view jsonInput, Registry& registry, entt::entity entity) {
            registry.template emplace<T>(entity, _load(jsonInput));
        }

        void load(std::string_view jsonInput, entt::registry& registry, entt::entity entity) override {
            _load(jsonInput, registry, entity);
        }

    };

} // Core
