module;

#include <optional>
#include <string>
#include <variant>

#include <entt/fwd.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

module Core.JsonTypeSaverAdapter;

import Core.Any;
import Core.BasicTypeTraits;
import Core.ClassReflection;
import Core.Log;
import Core.MapTypeTraits;
import Core.OptionalTypeTraits;
import Core.ReflectionContext;
import Core.TypeId;
import Core.TypeTraits;
import Core.VectorTypeTraits;

namespace Core {

	template<typename... AllTypes>
	class AnySaverMapper {

		template<typename T>
		static bool _trySaveAnyAsIntegral(Log& log,
			rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {

			constexpr auto min = std::numeric_limits<T>::min();
			constexpr auto max = std::numeric_limits<T>::max();

			const T& value{ *static_cast<const T*>(anyValue.getInstance()) };
			if constexpr (std::is_signed_v<T>) {
				outputValue.Set(static_cast<int32_t>(value));
			} else {
				outputValue.Set(static_cast<uint32_t>(value));
			}

			return true;
		}

		static bool _trySaveAnyAsString(Log& log,
			rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {

			const std::string& value{ *static_cast<const std::string*>(anyValue.getInstance()) };
			outputValue = rapidjson::Value(value.c_str(), allocator);

			return true;
		}

		template<typename T>
		static bool _trySaveAnyAs(Log& log,
			rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
			if (anyValue.getTypeId() != TypeId::get<T>()) {
				return false;
			}

			if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
				return _trySaveAnyAsIntegral<T>(log, outputValue, anyValue, allocator);
			} else if constexpr (std::is_same_v<T, std::string>) {
				return _trySaveAnyAsString(log, outputValue, anyValue, allocator);
			} else {
				const T& value{ *static_cast<const T*>(anyValue.getInstance()) };
				outputValue.Set(value);
			}

			return true;
		}

		template<typename Tail>
		static bool
		_trySaveAny(Log&, rapidjson::Value&, const Any&, rapidjson::Document::AllocatorType&) {
			return false;
		}

		template<typename Tail, typename T, typename... Types>
		static bool
		_trySaveAny(Log& log, rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
			return _trySaveAnyAs<T>(log, outputValue, anyValue, allocator) ||
				   _trySaveAny<Tail, Types...>(log, outputValue, anyValue, allocator);
		}

	public:
		static bool
		trySaveAny(Log& log, rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
			return _trySaveAny<void, AllTypes...>(log, outputValue, anyValue, allocator);
		}
	};

	using AnySaverMapperTypes =
		AnySaverMapper<std::string, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, double, float, bool>;

	void save(Log& log, rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator);

	template<typename T>
	void _save(Log&, rapidjson::Value&, const Any&, const T&, rapidjson::Document::AllocatorType&) {
		static_assert(false, "Unhandled type traits.");
	}

	void _save(Log&,
		rapidjson::Value&, const Any&, const VoidTypeTraits&,
		rapidjson::Document::AllocatorType&) {}

	void _save(Log& log,
		rapidjson::Value& outputValue, const Any& anyValue, const BasicTypeTraits& typeTraits,
		rapidjson::Document::AllocatorType& allocator) {
		const auto& context{ getCurrentReflectionContext() };
		const auto& typeId{ anyValue.getTypeId() };

		if (context.hasClass(typeId)) {
			outputValue.SetObject();

			const ClassReflection& classReflection{ context.getClass(typeId) };
			void* classInstance = anyValue.getInstance();
			for (const auto& property : classReflection.getProperties()) {
				Any propertyAny{ property.getTypeId(), property.getRawPointer(classInstance) };
				rapidjson::Value propertyValue;

				save(log, propertyValue, propertyAny, allocator);

				if (propertyValue.IsNull()) {
					continue;
				}

				rapidjson::Value propertyKey(property.getName().c_str(), allocator);
				outputValue.AddMember(std::move(propertyKey), std::move(propertyValue), allocator);
			}
		} else if (AnySaverMapperTypes::trySaveAny(log, outputValue, anyValue, allocator)) {
			// outputValue.SetBool(true);
		} else {
			logEntry(log, "Unhandled type.");
		}
	}

	void _save(Log& log,
		rapidjson::Value& outputValue, const Any& anyValue, const OptionalTypeTraits& typeTraits,
		rapidjson::Document::AllocatorType& allocator) {
		if (Any innerValue{ typeTraits.getFunc(anyValue) }; innerValue.getInstance() != nullptr) {
			save(log, outputValue, innerValue, allocator);
		}
	}

	void _save(Log& log,
		rapidjson::Value& outputValue, const Any& anyValue, const VectorTypeTraits& typeTraits,
		rapidjson::Document::AllocatorType& allocator) {
		outputValue.SetArray();
		typeTraits.forEachFunc(anyValue, [&log, &outputValue, &allocator](const Any& valueAny) {
			rapidjson::Value valueValue;
			save(log, valueValue, valueAny, allocator);

			outputValue.PushBack(std::move(valueValue), allocator);
		});
	}

	void _save(Log& log,
		rapidjson::Value& outputValue, const Any& anyValue, const MapTypeTraits& typeTraits,
		rapidjson::Document::AllocatorType& allocator) {
		outputValue.SetObject();
		typeTraits.forEachFunc(
			anyValue, [&log, &outputValue, &allocator](const Any& keyAny, const Any& valueAny) {
				rapidjson::Value keyValue;
				save(log, keyValue, keyAny, allocator);

				rapidjson::Value valueValue;
				save(log, valueValue, valueAny, allocator);

				outputValue.AddMember(keyValue, valueValue, allocator);
			});
	}

	void save(Log& log, rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
		std::visit(
			[&log, &outputValue, &anyValue, &allocator](auto&& typeTraits) {
				_save(log, outputValue, anyValue, typeTraits, allocator);
			},
			getTypeTraits(anyValue.getTypeId()));
	}

	std::string save(entt::registry& registry, const Any& anyValue) {
		Log log;
		const auto& reflectionContext{ getCurrentReflectionContext() };
		if (!reflectionContext.hasClass(anyValue.getTypeId())) {
			logEntry(log, "Class not registered.");
			return {};
		}

		const auto& classReflection{ reflectionContext.getClass(anyValue.getTypeId()) };
		const auto& className{ classReflection.getName() };

		rapidjson::Document doc;
		auto& allocator{ doc.GetAllocator() };

		rapidjson::Value classRootObject(rapidjson::kObjectType);
		classRootObject.SetObject();
		save(log, classRootObject, anyValue, allocator);

		rapidjson::Value classRootObjectKey(className.c_str(), allocator);
		doc.SetObject();
		doc.AddMember(classRootObjectKey, classRootObject, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter writer(buffer);
		doc.Accept(writer);

		return buffer.GetString();
	}

} // namespace Core
