module;

#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

module Ortha.Core.JsonTypeSaverAdapter;

import Ortha.Core.Log;
import Ortha.RTTI.Any;
import Ortha.RTTI.ReflectionContextStack;
import Ortha.RTTI.TemplateTypeTag;
import Ortha.RTTI.TypeId;
import Ortha.RTTI.TypeInfo;
import Ortha.RTTI.TypeInstance;
import entt;

namespace Ortha::Core {

	template<typename... AllTypes>
	class AnySaverMapper {

		template<typename T>
		static bool _trySaveAnyAsIntegral(Log& log,
			rapidjson::Value& outputValue, const RTTI::Any& anyValue, rapidjson::Document::AllocatorType& allocator) {

			constexpr auto min = std::numeric_limits<T>::min();
			constexpr auto max = std::numeric_limits<T>::max();

			const T& value{ *static_cast<const T*>(anyValue.getTypeInstance().getInstance()) };
			if constexpr (std::is_signed_v<T>) {
				outputValue.Set(static_cast<int32_t>(value));
			} else {
				outputValue.Set(static_cast<uint32_t>(value));
			}

			return true;
		}

		static bool _trySaveAnyAsString(Log& log,
			rapidjson::Value& outputValue, const RTTI::Any& anyValue, rapidjson::Document::AllocatorType& allocator) {

			const std::string& value{ *static_cast<const std::string*>(anyValue.getTypeInstance().getInstance()) };
			outputValue = rapidjson::Value(value.c_str(), allocator);

			return true;
		}

		template<typename T>
		static bool _trySaveAnyAs(Log& log,
			rapidjson::Value& outputValue, const RTTI::Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
			using namespace RTTI;

			if (anyValue.getTypeId() != TypeId::get<T>()) {
				return false;
			}

			if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
				return _trySaveAnyAsIntegral<T>(log, outputValue, anyValue, allocator);
			} else if constexpr (std::is_same_v<T, std::string>) {
				return _trySaveAnyAsString(log, outputValue, anyValue, allocator);
			} else {
				const T& value{ *static_cast<const T*>(anyValue.getTypeInstance().getInstance()) };
				outputValue.Set(value);
			}

			return true;
		}

		template<typename... Types>
		static bool
		_trySaveAny(Log& log, rapidjson::Value& outputValue, const RTTI::Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
			if constexpr (sizeof...(Types) == 1u) {
				return _trySaveAnyAs<Types...>(log, outputValue, anyValue, allocator);
			} else {
				return (_trySaveAny<Types>(log, outputValue, anyValue, allocator) || ...);
			}
		}

	public:
		static bool
		trySaveAny(Log& log, rapidjson::Value& outputValue, const RTTI::Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
			return _trySaveAny<AllTypes...>(log, outputValue, anyValue, allocator);
		}
	};

	using AnySaverMapperTypes =
		AnySaverMapper<std::string, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, double, float, bool>;

	void save(Log& log, rapidjson::Value& outputValue, const RTTI::Any& anyValue, rapidjson::Document::AllocatorType& allocator);

	void saveBasicJSON(Log& log,
		rapidjson::Value& outputValue, const RTTI::Any& anyValue,
		rapidjson::Document::AllocatorType& allocator) {
		using namespace RTTI;

		const auto& context{ ReflectionContextStack::getCurrentContext() };
		const auto& typeId{ anyValue.getTypeId() };
		const auto& typeInfo{ context.getTypeInfo(typeId) };
		if (!typeInfo) {
			return;
		}

		// TODO: A way to determine classes
		const auto& fields{ typeInfo->getFields() };
		if (!fields.empty()) {
			outputValue.SetObject();

			const void* classInstance = anyValue.getTypeInstance().getInstance();
			for (const auto& [propertyName, property] : fields) {
				Any propertyAny{ property.getTypeId(), property.getInstance(classInstance) };
				rapidjson::Value propertyValue;

				save(log, propertyValue, propertyAny, allocator);

				if (propertyValue.IsNull()) {
					continue;
				}

				rapidjson::Value propertyKey(propertyName.c_str(), allocator);
				outputValue.AddMember(std::move(propertyKey), std::move(propertyValue), allocator);
			}
		} else if (AnySaverMapperTypes::trySaveAny(log, outputValue, anyValue, allocator)) {
		} else {
			logEntry(log, "Unhandled type.");
		}
	}

	void saveOptionalJSON(Log& log,
		rapidjson::Value& outputValue, const RTTI::Any& anyValue,
		rapidjson::Document::AllocatorType& allocator) {
		using namespace RTTI;

		anyValue.getTypeInfo().visit(anyValue.getTypeInstance(), [&log, &outputValue, &allocator](const TypeInstance& valueAny) {
			save(log, outputValue, Any{ valueAny }, allocator);
		});
	}

	void saveSharedPtrJSON(Log& log,
		rapidjson::Value& outputValue, const RTTI::Any& anyValue,
		rapidjson::Document::AllocatorType& allocator) {
		using namespace RTTI;

		anyValue.getTypeInfo().visit(anyValue.getTypeInstance(), [&log, &outputValue, &allocator](const TypeInstance& valueAny) {
			save(log, outputValue, Any{ valueAny }, allocator);
		});
	}

	void saveVariantJSON(
		Log& log, rapidjson::Value& outputValue, const RTTI::Any& anyValue,
		rapidjson::Document::AllocatorType& allocator) {}

	void saveVectorJSON(Log& log,
		rapidjson::Value& outputValue, const RTTI::Any& anyValue,
		rapidjson::Document::AllocatorType& allocator) {
		using namespace RTTI;

		outputValue.SetArray();
		anyValue.getTypeInfo().visit(anyValue.getTypeInstance(), [&log, &outputValue, &allocator](const TypeInstance& valueAny) {
			rapidjson::Value valueValue;
			save(log, valueValue, Any{ valueAny }, allocator);
			outputValue.PushBack(std::move(valueValue), allocator);
		});
	}

	void saveMapJSON(Log& log,
		rapidjson::Value& outputValue, const RTTI::Any& anyValue,
		rapidjson::Document::AllocatorType& allocator) {
		using namespace RTTI;

		outputValue.SetObject();
		anyValue.getTypeInfo().visit(anyValue.getTypeInstance(), [&log, &outputValue, &allocator](const TypeInstance& valueAny) {
			// TODO: Digging out std::pair<TypeInstance, TypeInstance>
			rapidjson::Value keyValue;
			//save(log, keyValue, keyAny, allocator);

			rapidjson::Value valueValue;
			//save(log, valueValue, valueAny, allocator);

			outputValue.AddMember(keyValue, valueValue, allocator);
		});
	}

	void save(Log& log, rapidjson::Value& outputValue, const RTTI::Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
		using namespace RTTI;

		const auto& typeInfo{ anyValue.getTypeInfo() };
		if (typeInfo.getIsTemplate()) {
			const auto& templateTypeHandle{ typeInfo.getTemplateType() };
			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::map>>()) {
				return saveMapJSON(log, outputValue, anyValue, allocator);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::optional>>()) {
				return saveOptionalJSON(log, outputValue, anyValue, allocator);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::shared_ptr>>()) {
				return saveSharedPtrJSON(log, outputValue, anyValue, allocator);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::variant>>()) {
				return saveVariantJSON(log, outputValue, anyValue, allocator);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::vector>>()) {
				return saveVectorJSON(log, outputValue, anyValue, allocator);
			}

			if (templateTypeHandle == TypeId::get<TemplateTypeTag<std::basic_string>>()) {
				return saveBasicJSON(log, outputValue, anyValue, allocator);
			}
		}

		return saveBasicJSON(log, outputValue, anyValue, allocator);
	}

	std::string save(entt::registry& registry, const RTTI::Any& anyValue) {
		Log log;
		return save(log, anyValue);
	}

	std::string save(Log& log, const RTTI::Any& anyValue) {
		using namespace RTTI;

		const auto& reflectionContext{ ReflectionContextStack::getCurrentContext() };
		auto typeInfo = reflectionContext.getTypeInfo(anyValue.getTypeId());
		if (!typeInfo) {
			logEntry(log, "No type specified.");
			return {};
		}

		const auto& className{ typeInfo->getAlias() };
		if (!className) {
			logEntry(log, "No name specified.");
			return {};
		}

		rapidjson::Document doc;
		auto& allocator{ doc.GetAllocator() };

		rapidjson::Value classRootObject(rapidjson::kObjectType);
		classRootObject.SetObject();
		save(log, classRootObject, anyValue, allocator);

		rapidjson::Value classRootObjectKey(className->c_str(), allocator);
		doc.SetObject();
		doc.AddMember(classRootObjectKey, classRootObject, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter writer(buffer);
		doc.Accept(writer);

		return buffer.GetString();
	}

} // namespace Ortha::Core
