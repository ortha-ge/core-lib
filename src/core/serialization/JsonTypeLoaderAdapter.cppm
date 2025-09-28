module;

#include <string_view>

export module Core.JsonTypeLoaderAdapter;

import Core.Any;
import Core.Log;
import Core.ReflectionContext;
import Core.TypeLoader;
import entt;

export namespace Core {

	void load(entt::registry&, const ReflectionContext&, std::string_view, Any&);
	void load(Log&, const ReflectionContext&, std::string_view, Any&);
	Any load(entt::registry&, const ReflectionContext&, std::string_view);
	Any load(Log&, const ReflectionContext&, std::string_view);

	template<typename T>
	class JsonTypeLoaderAdapter : public TypeLoaderAdapter {
	public:
		T _loadComponent(entt::registry& registry, entt::entity entity, std::string_view jsonInput) {
			auto& reflectionContext{ getCurrentReflectionContext() };
			Core::reflectIfValidType<T>(reflectionContext);

			T instance{};
			Any any(instance);
			Core::load(registry, reflectionContext, jsonInput, any);

			logEntries(registry, entity, reflectionContext.moveLog());

			return instance;
		}

		template<typename Registry>
		void _load(Registry& registry, entt::entity entity, std::string_view jsonInput) {
			registry.template emplace<T>(entity, _loadComponent(registry, entity, jsonInput));
		}

		void load(entt::registry& registry, entt::entity entity, std::string_view jsonInput) override {
			_load(registry, entity, jsonInput);
		}
	};

} // namespace Core
