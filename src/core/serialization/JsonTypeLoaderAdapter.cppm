module;

#include <string_view>

#include <entt/fwd.hpp>

export module Core.JsonTypeLoaderAdapter;

import Core.Any;
import Core.ReflectionContext;
import Core.TypeLoader;

export namespace Core {

	void load(const ReflectionContext&, std::string_view, Any&);
	Any load(const ReflectionContext&, std::string_view);

    template <typename T>
    class JsonTypeLoaderAdapter : public TypeLoaderAdapter {
    public:

        T _load(std::string_view jsonInput) {
			auto& reflectionContext{getCurrentReflectionContext()};
			Core::reflect<T>(reflectionContext);

			T instance{};
			Any any(instance);
			Core::load(reflectionContext, jsonInput, any);
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
