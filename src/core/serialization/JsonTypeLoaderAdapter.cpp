module;

#include <map>
#include <print>
#include <variant>

#include <rapidjson/document.h>

module Ortha.Core.JsonTypeLoaderAdapter;
import Ortha.Core.Log;
import Ortha.RTTI.Any;
import Ortha.RTTI.TemplateTypeTag;
import Ortha.RTTI.TypeHandle;
import Ortha.RTTI.TypeId;
import Ortha.RTTI.TypeInfo;
import Ortha.RTTI.TypeInstance;
import entt;

namespace Ortha::Core {

	template<typename... AllTypes>
	class AnyLoaderMapper {

		template<typename T>
		static bool _tryLoadAnyAsIntegral(Log& log, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
			if (!inputValue.IsNumber()) {
				return false;
			}

			constexpr auto min = std::numeric_limits<T>::min();
			constexpr auto max = std::numeric_limits<T>::max();

			T& value{ *static_cast<T*>(anyValue.getTypeInstance().getInstance()) };

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

		static bool _tryLoadAnyAsString(Log& log, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
			if (!inputValue.IsString()) {
				return false;
			}

			std::string& value{ *static_cast<std::string*>(anyValue.getTypeInstance().getInstance()) };
			value = inputValue.GetString();
			return true;
		}

		static bool _tryLoadAnyAsTypeId(Log& log, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
			using namespace RTTI;
			const auto& reflectionContext{ ReflectionContextStack::getCurrentContext() };
			std::string typeName{ inputValue.GetString() };
			if (auto typeId = reflectionContext.getTypeInfoByName(typeName)) {
				TypeId& typeIdValue{ *static_cast<TypeId*>(anyValue.getTypeInstance().getInstance()) };
				typeIdValue = typeId->getTypeId();
			}

			return true;
		}

		template<typename T>
		static bool _tryLoadAnyAs(Log& log, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
			if (anyValue.getTypeId() != RTTI::TypeId::get<T>()) {
				return false;
			}

			if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
				return _tryLoadAnyAsIntegral<T>(log, inputValue, anyValue);
			} else if constexpr (std::is_same_v<T, std::string>) {
				return _tryLoadAnyAsString(log, inputValue, anyValue);
			} else if constexpr (std::is_same_v<T, RTTI::TypeId>) {
				return _tryLoadAnyAsTypeId(log, inputValue, anyValue);
			} else {
				T& value{ *static_cast<T*>(anyValue.getTypeInstance().getInstance()) };
				value = inputValue.Get<T>();
			}

			return true;
		}

		template<typename... Types>
		static bool _tryLoadAny(Log& log, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
			if constexpr (sizeof...(Types) == 1u) {
				return _tryLoadAnyAs<Types...>(log, inputValue, anyValue);
			} else {
				return (_tryLoadAny<Types>(log, inputValue, anyValue) || ...);
			}
		}

	public:
		static bool tryLoadAny(Log& log, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
			return _tryLoadAny<AllTypes...>(log, inputValue, anyValue);
		}
	};

	using AnyLoaderMapperTypes = AnyLoaderMapper<
		RTTI::TypeId, std::string, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, double, float, bool>;

	bool loadJSON(Log&, const RTTI::ReflectionContext&, const rapidjson::Value&, const rapidjson::Value&, RTTI::Any&);

	bool loadBasicJSON(Log& log,
		const RTTI::ReflectionContext& reflectionContext, const rapidjson::Value&, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
		using namespace RTTI;

		const auto& typeInfo{ anyValue.getTypeInfo() };

		if (inputValue.IsObject()) {
			for (const auto& [propertyName, property] : typeInfo.getFields()) {
				const auto& propertyTypeInfo{ getTypeInfo(property.getTypeId()) };
				const bool isOptionalProperty = propertyTypeInfo.getIsTemplate()
					&& propertyTypeInfo.getTemplateType() == TypeId::get<TemplateTypeTag<std::optional>>();

				auto it = inputValue.FindMember(propertyName.c_str());
				if (it == inputValue.MemberEnd()) {
					if (!isOptionalProperty) {
						logEntry(log, "Missing required property: {}", propertyName);
					}

					continue;
				}

				Any propertyAny{ property.getTypeId(), property.getInstance(anyValue.getTypeInstance().getInstance()) };
				loadJSON(log, reflectionContext, it->name, it->value, propertyAny);
			}

			return true;
		}

		// TODO: A way to identify enums
		if (typeInfo.getTypeId() != TypeId::get<std::string>() &&
			typeInfo.getTypeId() != TypeId::get<TypeId>() &&
			inputValue.IsString()) {
			const char* enumString = inputValue.GetString();
			if (!typeInfo.hasConstant(enumString)) {
				logEntry(log, "Couldn't find enumerator with name '{}'.", enumString);
				return false;
			}

			Any enumValue{ typeInfo.getConstant(enumString) };
			anyValue = enumValue;
			return true;
		}

		if (AnyLoaderMapperTypes::tryLoadAny(log, inputValue, anyValue)) {
			return true;
		}

		logEntry(log, "Unhandled type.");
		return false;
	}

	bool loadOptionalJSON(Log& log,
		const RTTI::ReflectionContext& reflectionContext, const rapidjson::Value& inputKey, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
		using namespace RTTI;

		const auto& typeInfo{ anyValue.getTypeInfo() };
		const auto& templateTypeHandle{ typeInfo.getTemplateArgsType(0zu) };

		Any wrappedTypeAny{ TypeId(templateTypeHandle) };
		bool result = loadJSON(log, reflectionContext, inputKey, inputValue, wrappedTypeAny);
		anyValue = wrappedTypeAny;
		return result;
	}

	bool loadSharedPtrJSON(Log& log,
		const RTTI::ReflectionContext& reflectionContext, const rapidjson::Value& inputKey, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
		using namespace RTTI;

		const auto& typeInfo{ anyValue.getTypeInfo() };
		const auto& templateTypeHandle{ typeInfo.getTemplateArgsType(0zu) };

		Any wrappedTypeAny{ TypeId(templateTypeHandle) };
		bool result = loadJSON(log, reflectionContext, inputKey, inputValue, wrappedTypeAny);
		anyValue = wrappedTypeAny;
		return result;
	}

	// Load variant with root key used for variant type.
	// Load variant from Object JSON that has 'variant' string type.
	// Load variant with trial and error.

	std::optional<RTTI::TypeId> validateTypeNameInVariantTypes(const RTTI::ReflectionContext& reflectionContext, const std::string& typeName, const RTTI::TypeInfo& variantTypeInfo) {
		using namespace RTTI;
		if (auto typeInfo = std::static_pointer_cast<TypeInfo>(reflectionContext.getTypeInfoByName(typeName))) {
			const auto& typeId = typeInfo->getTypeId();
			for (size_t i = 0; i < variantTypeInfo.getTemplateArgsCount(); ++i) {
				if (variantTypeInfo.getTemplateArgsType(i) == typeId) {
					return typeId;
				}
			}
		}

		return std::nullopt;
	}

	bool loadJSONVariantByTypeId(Log& log,
			const RTTI::ReflectionContext& reflectionContext, RTTI::TypeId typeId, const rapidjson::Value& inputKey, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
		using namespace RTTI;

		Any variantTypeAny(std::move(typeId));
		if (loadJSON(log, reflectionContext, inputKey, inputValue, variantTypeAny)) {
			anyValue = variantTypeAny;
			return true;
		}

		return false;
	}

	bool loadVariantJSON(Log& log,
			const RTTI::ReflectionContext& reflectionContext, const rapidjson::Value& inputKey, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
		using namespace RTTI;

		const auto& variantTypeInfo{ anyValue.getTypeInfo() };

		// Search for the variant load type.
		std::optional<TypeId> loadType = validateTypeNameInVariantTypes(reflectionContext, inputKey.GetString(), variantTypeInfo);
		if (!loadType && inputValue.IsObject()) {
			auto variantTypeIt = inputValue.FindMember("variant");
			if (variantTypeIt == inputValue.MemberEnd()) {
				logEntry(log, "Missing required property: variant.");
				return false;
			}

			if (!variantTypeIt->value.IsString()) {
				logEntry(log, "VariantType requires string type.");
				return false;
			}

			loadType = validateTypeNameInVariantTypes(reflectionContext, variantTypeIt->value.GetString(), variantTypeInfo);
			if (!loadType) {
				logEntry(log, "Variant Type not found in possible types.");
				return false;
			}
		}

		if (loadType) {
			return loadJSONVariantByTypeId(log, reflectionContext, *loadType, inputKey, inputValue, anyValue);
		}

		// Trial and error.
		const auto& typeInfo(anyValue.getTypeInfo());
		for (size_t i = 0; i < typeInfo.getTemplateArgsCount(); ++i) {
			const auto& templateTypeHandle{ typeInfo.getTemplateArgsType(i) };
			if (loadJSONVariantByTypeId(log, reflectionContext, TypeId(templateTypeHandle), inputKey, inputValue, anyValue)) {
				return true;
			}
		}

		return false;
	}

	bool loadVectorJSON(Log& log,
		const RTTI::ReflectionContext& reflectionContext, const rapidjson::Value& inputKey, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
		using namespace RTTI;

		if (!inputValue.IsArray()) {
			logEntry(log, "Not an array.");
			return false;
		}

		const auto& typeInfo(anyValue.getTypeInfo());
		const auto& elementTypeHandle{ typeInfo.getTemplateArgsType(0zu) };

		const auto& jsonArray{ inputValue.GetArray() };
		for (uint32_t i = 0; i < jsonArray.Size(); ++i) {
			Any wrappedTypeAny{ TypeId(elementTypeHandle) };
			if (!loadJSON(log, reflectionContext, rapidjson::Value(i), jsonArray[i], wrappedTypeAny)) {
				return false;
			}
			typeInfo.emplace(anyValue.getTypeInstance(), wrappedTypeAny.getTypeInstance());
		}

		return true;
	}

	bool loadMapJSON(Log& log,
		const RTTI::ReflectionContext& reflectionContext, const rapidjson::Value& inputKey, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
		using namespace RTTI;

		if (!inputValue.IsObject()) {
			logEntry(log, "Not an object.");
			return false;
		}

		const auto& typeInfo(anyValue.getTypeInfo());
		const auto& keyTypeHandle{ typeInfo.getTemplateArgsType(0zu) };
		const auto& mappedTypeHandle{ typeInfo.getTemplateArgsType(1zu) };

		for (const auto& objectElement : inputValue.GetObject()) {
			// Key
			Any wrappedKeyAny{ TypeId(keyTypeHandle) };
			loadJSON(log, reflectionContext, objectElement.name, objectElement.name, wrappedKeyAny);

			// Value
			Any wrappedValueAny { TypeId(mappedTypeHandle) };
			loadJSON(log, reflectionContext, objectElement.name, objectElement.value, wrappedValueAny);

			// TODO: Make into pair
			auto wrappedPair = std::make_pair(wrappedKeyAny.getTypeInstance(), wrappedValueAny.getTypeInstance());
			Any wrappedPairAny{ std::move(wrappedPair) };

			typeInfo.emplace(anyValue.getTypeInstance(), wrappedPairAny.getTypeInstance());
		}

		return true;
	}

	bool loadJSON(Log& log, const RTTI::ReflectionContext& reflectionContext, const rapidjson::Value& inputKey, const rapidjson::Value& inputValue, RTTI::Any& anyValue) {
		using namespace RTTI;

		const auto& typeInfo{ getTypeInfo(anyValue.getTypeId()) };
		if (typeInfo.getIsTemplate()) {
			const auto& templateTypeHandle{ typeInfo.getTemplateType() };
			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::map>>()) {
				return loadMapJSON(log, reflectionContext, inputKey, inputValue, anyValue);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::optional>>()) {
				return loadOptionalJSON(log, reflectionContext, inputKey, inputValue, anyValue);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::shared_ptr>>()) {
				return loadSharedPtrJSON(log, reflectionContext, inputKey, inputValue, anyValue);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::variant>>()) {
				return loadVariantJSON(log, reflectionContext, inputKey, inputValue, anyValue);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::vector>>()) {
				return loadVectorJSON(log, reflectionContext, inputKey, inputValue, anyValue);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::basic_string>>()) {
				return loadBasicJSON(log, reflectionContext, inputKey, inputValue, anyValue);
			}
		}

		return loadBasicJSON(log, reflectionContext, inputKey, inputValue, anyValue);
	}



	std::string validateClassRoot(Log& log, const RTTI::ReflectionContext& reflectionContext, const rapidjson::Document& doc, const RTTI::TypeId& typeId);

	std::string validateBasicClassRoot(Log& log, const RTTI::ReflectionContext& reflectionContext,  const rapidjson::Document& doc, const RTTI::TypeId& typeId) {
		using namespace RTTI;
		// if (!reflectionContext.hasClass(typeId)) {
		// 	logEntry(log, "Class not registered.");
		// 	return {};
		// }

		const auto& classReflection{ reflectionContext.getTypeInfo(typeId) };
		const auto& className{ classReflection->getAlias() };
		if (!className) {
			return {};
		}

		auto classRootObjectIt = doc.FindMember(className->c_str());
		if (classRootObjectIt == doc.MemberEnd()) {
			return {};
		}

		return *className;
	}

	std::string validateVariantClassRoot(Log& log, const RTTI::ReflectionContext& reflectionContext,  const rapidjson::Document& doc, const RTTI::TypeId& typeId) {
		using namespace RTTI;
		const auto& typeInfo{ getTypeInfo(typeId) };
		for (size_t i = 0; i < typeInfo.getTemplateArgsCount(); ++i) {
			const auto& variantTypeId{ typeInfo.getTemplateArgsType(i) };
			auto variantTypeInfo = reflectionContext.getTypeInfo(variantTypeId);
			if (!variantTypeInfo) {
				continue;
			}

			if (auto alias = variantTypeInfo->getAlias()) {
				std::string validatedName = validateClassRoot(log, reflectionContext, doc, variantTypeId);
				if (!validatedName.empty()) {
					return validatedName;
				}
			}
		}

		return {};
	}

	std::string validateClassRoot(Log& log, const RTTI::ReflectionContext& reflectionContext, const rapidjson::Document& doc, const RTTI::TypeId& typeId) {
		using namespace RTTI;
		if (getTypeInfo(typeId).getTemplateType() == TypeId::get<TemplateTypeTag<std::variant>>()) {
			return validateVariantClassRoot(log, reflectionContext, doc, typeId);
		}

		return validateBasicClassRoot(log, reflectionContext, doc, typeId);
	}

	std::string validateClassRoot(Log& log, const RTTI::ReflectionContext& reflectionContext, const rapidjson::Document& doc, RTTI::Any& anyValue) {
		return validateClassRoot(log, reflectionContext, doc, anyValue.getTypeId());
	}

	void load(entt::registry& registry, const RTTI::ReflectionContext& reflectionContext, std::string_view jsonInput, RTTI::Any& anyValue) {
		Log log;
		load(log, reflectionContext, jsonInput, anyValue);
		logEntries(registry, std::move(log));
	}

	void load(Log& log, const RTTI::ReflectionContext& reflectionContext, std::string_view jsonInput, RTTI::Any& anyValue) {
		rapidjson::Document doc;
		doc.Parse(jsonInput.data(), jsonInput.length());

		const std::string className{ validateClassRoot(log, reflectionContext, doc, anyValue) };

		auto classRootObjectIt = doc.FindMember(className.c_str());
		if (classRootObjectIt == doc.MemberEnd()) {
			logEntry(log, "Couldn't find matching class root '{}' object.", className);
			return;
		}

		loadJSON(log, reflectionContext, classRootObjectIt->name, classRootObjectIt->value, anyValue);
	}

	RTTI::Any load(entt::registry& registry, const RTTI::ReflectionContext& reflectionContext, std::string_view jsonInput) {
		Log log;
		RTTI::Any result = load(log, reflectionContext, jsonInput);
		logEntries(registry, std::move(log));
		return result;
	}

	RTTI::Any load(Log& log, const RTTI::ReflectionContext& reflectionContext, std::string_view jsonInput) {
		using namespace RTTI;

		rapidjson::Document doc;
		doc.Parse(jsonInput.data(), jsonInput.length());

		if (doc.MemberCount() != 1) {
			logEntry(log, "Requires a single root object with the name of the type represented.");
			return {};
		}

		auto rootClassObjectIt = doc.MemberBegin();
		const auto& rootClassNameValue{ rootClassObjectIt->name };
		const char* rootClassName = rootClassNameValue.GetString();

		auto typeInfo = reflectionContext.getTypeInfoByName(rootClassName);
		if (!typeInfo) {
			logEntry(log, "TypeId not found with name '{}'.", rootClassName);
			return {};
		}

		Any instance{ typeInfo->getTypeId() };
		loadJSON(log, reflectionContext, rootClassObjectIt->name, rootClassObjectIt->value, instance);
		return instance;
	}

} // namespace Ortha::Core
