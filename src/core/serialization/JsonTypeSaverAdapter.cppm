module;

#include <string_view>
#include <string>

#include <entt/fwd.hpp>

export module Core.JsonTypeSaverAdapter;

import Core.Any;
import Core.TypeSaver;

export namespace Core {

    std::string save(const Any& anyValue);

    template <typename T>
    class JsonTypeSaverAdapter : public TypeSaverAdapter {
    public:

        std::string _save(const T& instance) {
            Any any(instance);
            return Core::save(any);
        }

        template <typename Registry>
        void _save(std::string_view jsonInput, Registry& registry, entt::entity entity) {
            //registry.template emplace<T>(entity, _save(jsonInput));
        }

        void save(std::string_view jsonInput, entt::registry& registry, entt::entity entity) override {
            //_save(jsonInput, registry, entity);
        }

    };

} // Core
