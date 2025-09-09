module;

#include <map>
#include <print>
#include <variant>

#include <entt/entt.hpp>

#include <rapidjson/document.h>

module Core.JsonTypeLoaderAdapter;

import Core.BasicTypeTraits;
import Core.ClassReflection;
import Core.EnumReflection;
import Core.Log;
import Core.MapTypeTraits;
import Core.OptionalTypeTraits;
import Core.ReflectionContext;
import Core.TypeId;
import Core.TypeTraits;
import Core.VectorTypeTraits;

namespace Core {

	template<typename... AllTypes>
	class AnyLoaderMapper {

		template<typename T>
		static bool _tryLoadAnyAsIntegral(Log& log, const rapidjson::Value& inputValue, Any& anyValue) {

			constexpr auto min = std::numeric_limits<T>::min();
			constexpr auto max = std::numeric_limits<T>::max();

			T& value{ *static_cast<T*>(anyValue.getInstance()) };

			if constexpr (std::is_signed_v<T>) {
				auto intValue = inputValue.Get<int32_t>();
				if (intValue < min || intValue > max) {
					logEntry(log, "Exceeded range.");
					return true;
				}

				value = intValue;
			} else {
				auto uintValue = inputValue.Get<uint32_t>();
				if (uintValue < min || uintValue > max) {
					logEntry(log, "Exceeded range.");
					return true;
				}

				value = uintValue;
			}

			return true;
		}

		static bool _tryLoadAnyAsString(Log& log, const rapidjson::Value& inputValue, Any& anyValue) {
			std::string& value{ *static_cast<std::string*>(anyValue.getInstance()) };
			value = inputValue.GetString();
			return true;
		}

		static bool _tryLoadAnyAsTypeId(Log& log, const rapidjson::Value& inputValue, Any& anyValue) {
			const auto& reflectionContex{ getCurrentReflectionContext() };
			std::string typeName{ inputValue.GetString() };
			if (auto typeId = reflectionContex.getTypeIdByName(typeName)) {
				TypeId& typeIdValue{ *static_cast<TypeId*>(anyValue.getInstance()) };
				typeIdValue = *typeId;
			}

			return true;
		}

		template<typename T>
		static bool _tryLoadAnyAs(Log& log, const rapidjson::Value& inputValue, Any& anyValue) {
			if (anyValue.getTypeId() != TypeId::get<T>()) {
				return false;
			}

			if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
				return _tryLoadAnyAsIntegral<T>(log, inputValue, anyValue);
			} else if constexpr (std::is_same_v<T, std::string>) {
				return _tryLoadAnyAsString(log, inputValue, anyValue);
			} else if constexpr (std::is_same_v<T, TypeId>) {
				return _tryLoadAnyAsTypeId(log, inputValue, anyValue);
			} else {
				T& value{ *static_cast<T*>(anyValue.getInstance()) };
				value = inputValue.Get<T>();
			}

			return true;
		}

		template<typename Tail>
		static bool _tryLoadAny(Log&, const rapidjson::Value&, Any&) {
			return false;
		}

		template<typename Tail, typename T, typename... Types>
		static bool _tryLoadAny(Log& log, const rapidjson::Value& inputValue, Any& anyValue) {
			return _tryLoadAnyAs<T>(log, inputValue, anyValue) || _tryLoadAny<Tail, Types...>(log, inputValue, anyValue);
		}

	public:
		static bool tryLoadAny(Log& log, const rapidjson::Value& inputValue, Any& anyValue) {
			return _tryLoadAny<void, AllTypes...>(log, inputValue, anyValue);
		}
	};

	using AnyLoaderMapperTypes = AnyLoaderMapper<
		TypeId, std::string, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, double, float, bool>;

	void load(Log&, const ReflectionContext&, const rapidjson::Value&, Any&);

	template<typename T>
	void
	_load(Log&, const ReflectionContext&, const rapidjson::Value&, Any&, const T&) {
		static_assert(false, "Unhandled type traits.");
	}

	void _load(Log&,
		const ReflectionContext&, const rapidjson::Value&, Any&,
		const VoidTypeTraits&) {}

	void _load(Log& log,
		const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue,
		const BasicTypeTraits& typeTraits) {

		const auto& typeId{ anyValue.getTypeId() };

		if (reflectionContext.hasClass(typeId)) {
			const ClassReflection& classReflection{ reflectionContext.getClass(typeId) };
			void* classInstance = anyValue.getInstance();
			for (const auto& property : classReflection.getProperties()) {
				const auto& propertyTypeTraits{ getTypeTraits(property.getTypeId()) };
				const bool isOptionalProperty = std::holds_alternative<OptionalTypeTraits>(propertyTypeTraits);

				auto it = inputValue.FindMember(property.getName().c_str());
				if (it == inputValue.MemberEnd()) {
					if (!isOptionalProperty) {
						logEntry(log, "Missing required property: {}", property.getName());
					}

					continue;
				}

				Any propertyAny{ property.getTypeId(), property.getRawPointer(classInstance) };
				load(log, reflectionContext, it->value, propertyAny);
			}
		} else if (reflectionContext.hasEnum(typeId)) {
			const EnumReflection& enumReflection{ reflectionContext.getEnum(typeId) };
			if (!inputValue.IsString()) {
				logEntry(log, "Enumerator requires string type.");
				return;
			}

			const char* enumString = inputValue.GetString();
			if (!enumReflection.hasEnumerator(enumString)) {
				logEntry(log, "Couldn't find enumerator with name '{}'.", enumString);
				return;
			}

			Any enumValue{ enumReflection.getEnumeratorValue(enumString) };
			anyValue = enumValue;
		} else if (AnyLoaderMapperTypes::tryLoadAny(log, inputValue, anyValue)) {

		} else {
			logEntry(log, "Unhandled type.");
		}
	}

	void _load(Log& log,
		const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue,
		const OptionalTypeTraits& typeTraits) {
		Any wrappedTypeAny(typeTraits.elementType);
		load(log, reflectionContext, inputValue, wrappedTypeAny);
		anyValue = wrappedTypeAny;
	}

	void _load(Log& log,
		const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue,
		const VectorTypeTraits& typeTraits) {
		if (!inputValue.IsArray()) {
			logEntry(log, "Not an array.");
			return;
		}

		Any anyVector(TypeId::get<std::vector<Any>>());
		std::vector<Any>& wrappedVector = *static_cast<std::vector<Any>*>(anyVector.getInstance());
		wrappedVector.reserve(inputValue.Size());

		for (const auto& inputElement : inputValue.GetArray()) {
			Any wrappedTypeAny(typeTraits.elementType);
			load(log, reflectionContext, inputElement, wrappedTypeAny);
			wrappedVector.emplace_back(std::move(wrappedTypeAny));
		}

		anyValue = anyVector;
	}

	void _load(Log& log,
		const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue,
		const MapTypeTraits& typeTraits) {
		if (!inputValue.IsObject()) {
			logEntry(log, "Not an object.");
			return;
		}

		Any anyMap(TypeId::get<std::map<Any, Any>>());
		std::map<Any, Any>& wrappedMap = *static_cast<std::map<Any, Any>*>(anyMap.getInstance());

		for (const auto& objectElement : inputValue.GetObject()) {
			// Key
			Any wrappedKeyAny(TypeId::get<std::string>());
			std::string& wrappedKey{ *static_cast<std::string*>(wrappedKeyAny.getInstance()) };
			wrappedKey = objectElement.name.GetString();

			// Value
			Any wrappedValueAny(typeTraits.valueType);
			load(log, reflectionContext, objectElement.value, wrappedValueAny);
			wrappedMap[wrappedKeyAny] = std::move(wrappedValueAny);
		}

		anyValue = anyMap;
	}

	void load(Log& log, const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue) {
		std::visit(
			[&log, &reflectionContext, &inputValue, &anyValue](auto&& typeTraits) {
				_load(log, reflectionContext, inputValue, anyValue, typeTraits);
			},
			getTypeTraits(anyValue.getTypeId()));
	}

	void load(entt::registry& registry, const ReflectionContext& reflectionContext, std::string_view jsonInput, Any& anyValue) {
		Log log;
		load(log, reflectionContext, jsonInput, anyValue);
		logEntries(registry, std::move(log));
	}

	void load(Log& log, const ReflectionContext& reflectionContext, std::string_view jsonInput, Any& anyValue) {
		if (!reflectionContext.hasClass(anyValue.getTypeId())) {
			logEntry(log, "Class not registered.");
			return;
		}

		const auto& classReflection{ reflectionContext.getClass(anyValue.getTypeId()) };
		const auto& className{ classReflection.getName() };

		rapidjson::Document doc;
		doc.Parse(jsonInput.data(), jsonInput.length());

		auto classRootObjectIt = doc.FindMember(className.c_str());
		if (classRootObjectIt == doc.MemberEnd()) {
			logEntry(log, "Couldn't find matching class root '{}' object.", className);
			return;
		}

		load(log, reflectionContext, classRootObjectIt->value, anyValue);
	}

	Any load(entt::registry& registry, const ReflectionContext& reflectionContext, std::string_view jsonInput) {
		Log log;
		Any result = load(log, reflectionContext, jsonInput);
		logEntries(registry, std::move(log));
		return result;
	}

	Any load(Log& log, const ReflectionContext& reflectionContext, std::string_view jsonInput) {
		rapidjson::Document doc;
		doc.Parse(jsonInput.data(), jsonInput.length());

		if (doc.MemberCount() != 1) {
			logEntry(log, "Requires a single root object with the name of the type represented.");
			return {};
		}

		auto rootClassObjectIt = doc.MemberBegin();
		const auto& rootClassNameValue{ rootClassObjectIt->name };
		const char* rootClassName = rootClassNameValue.GetString();

		auto typeId = reflectionContext.getTypeIdByName(rootClassName);
		if (!typeId) {
			logEntry(log, "TypeId not found with name '{}'.", rootClassName);
			return {};
		}

		if (!reflectionContext.hasClass(*typeId)) {
			logEntry(log, "TypeId is not a registered class.");
			return {};
		}

		Any instance(*typeId);
		load(log, reflectionContext, rootClassObjectIt->value, instance);
		return instance;
	}

} // namespace Core
