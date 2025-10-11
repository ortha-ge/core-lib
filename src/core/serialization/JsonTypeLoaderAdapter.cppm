module;

#include <string_view>

export module Ortha.Core.JsonTypeLoaderAdapter;

import Ortha.RTTI.Any;
import Ortha.RTTI.ReflectionContext;
import Ortha.RTTI.ReflectionContextStack;
import Ortha.Core.Log;
import Ortha.RTTI.Reflect;
import Ortha.Core.TypeLoader;
import entt;

export namespace Ortha::Core {

	void load(entt::registry&, const RTTI::ReflectionContext&, std::string_view, RTTI::Any&);
	void load(Log&, const RTTI::ReflectionContext&, std::string_view, RTTI::Any&);
	RTTI::Any load(entt::registry&, const RTTI::ReflectionContext&, std::string_view);
	RTTI::Any load(Log&, const RTTI::ReflectionContext&, std::string_view);

	template<typename T>
	class JsonTypeLoaderAdapter : public TypeLoaderAdapter {
	public:
		T _loadComponent(entt::registry& registry, entt::entity entity, std::string_view jsonInput) {
			using namespace Ortha::RTTI;
			auto& reflectionContext{ ReflectionContextStack::getCurrentContext() };
			ensureReflected<T>(reflectionContext);

			T instance{};
			Any any(instance);
			Core::load(registry, reflectionContext, jsonInput, any);

			//logEntries(registry, entity, reflectionContext.moveLog());

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

} // namespace Ortha::Core
