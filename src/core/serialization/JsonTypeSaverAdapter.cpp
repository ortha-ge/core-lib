module;

#include <optional>
#include <variant>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

module Core.JsonTypeSaverAdapter;

import Core.Any;
import Core.ClassReflection;
import Core.ReflectionContext;
import Core.TypeId;

namespace Core {

    template <typename... AllTypes>
    class AnySaverMapper {

        template <typename T>
        static bool _trySaveAnyAsIntegral(rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {

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

        static bool _trySaveAnyAsString(rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {

            const std::string& value{ *static_cast<const std::string*>(anyValue.getInstance()) };
            outputValue = rapidjson::Value(value.c_str(), allocator);

            return true;
        }

        template <typename T>
        static bool _trySaveAnyAs(rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
            if (anyValue.getTypeId() != TypeId::get<T>()) {
                return false;
            }

            if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
                return _trySaveAnyAsIntegral<T>(outputValue, anyValue, allocator);
            } else if constexpr (std::is_same_v<T, std::string>) {
                return _trySaveAnyAsString(outputValue, anyValue, allocator);
            } else {
                const T& value{ *static_cast<const T*>(anyValue.getInstance()) };
                outputValue.Set(value);
            }

            return true;
        }

        // Output is a template as a tail for empty Types...
        template <typename Tail>
        static bool _trySaveAny(rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
            return false;
        }

        template <typename Tail, typename T, typename ... Types>
        static bool _trySaveAny(rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
            return _trySaveAnyAs<T>(outputValue, anyValue, allocator)
                || _trySaveAny<Tail, Types...>(outputValue, anyValue, allocator);
        }

    public:

        static bool trySaveAny(rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
            return _trySaveAny<void, AllTypes...>(outputValue, anyValue, allocator);
        }

    };

    using AnySaverMapperTypes = AnySaverMapper<
        std::string,
        int8_t,
        uint8_t,
        int16_t,
        uint16_t,
        int32_t,
        uint32_t,
        double,
        float,
        bool
    >;

    void save(rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator);

    template <typename T>
    void _save(rapidjson::Value&, const Any&, const T&, rapidjson::Document::AllocatorType&) {
        static_assert(false, "Unhandled type traits.");
    }

    void _save(rapidjson::Value& outputValue, const Any& anyValue, const TypeTraits& typeTraits, rapidjson::Document::AllocatorType& allocator) {
        const auto& context{ getCurrentReflectionContext() };
        const auto& typeId{ anyValue.getTypeId() };

        if (context.hasClass(typeId)) {
            outputValue.SetObject();

            const ClassReflection& classReflection{ context.getClass(typeId) };
            void* classInstance = anyValue.getInstance();
            for (const auto& property : classReflection.getProperties()) {
                Any propertyAny{ property.getTypeId(), property.getRawPointer(classInstance) };
                rapidjson::Value propertyValue;

                save(propertyValue, propertyAny, allocator);

                if (propertyValue.IsNull()) {
                    continue;
                }

                rapidjson::Value propertyKey(property.getName().c_str(), allocator);
                outputValue.AddMember(std::move(propertyKey), std::move(propertyValue), allocator);
            }
        } else if (AnySaverMapperTypes::trySaveAny(outputValue, anyValue, allocator)) {
            //outputValue.SetBool(true);
        } else {
            printf("Unhandled type\n");
        }
    }

    void _save(rapidjson::Value& outputValue, const Any& anyValue, const OptionalTypeTraits& typeTraits, rapidjson::Document::AllocatorType& allocator) {
        if (void* wrappedInstance = typeTraits.mGet(anyValue.getInstance())) {
            Any wrappedAnyValue{ typeTraits.mWrappedType, wrappedInstance };
            save(outputValue, wrappedAnyValue, allocator);
        }
    }

    void _save(rapidjson::Value& outputValue, const Any& anyValue, const VectorTypeTraits& typeTraits, rapidjson::Document::AllocatorType& allocator) {
        outputValue.SetArray();
        typeTraits.mForEach(anyValue.getInstance(), [&outputValue, &typeTraits, &allocator](const void* value) {
            Any valueAny{ typeTraits.mWrappedType, const_cast<void*>(value) };

            rapidjson::Value valueValue;
            save(valueValue, valueAny, allocator);

            outputValue.PushBack(std::move(valueValue), allocator);
        });
    }

    void _save(rapidjson::Value& outputValue, const Any& anyValue, const MapTypeTraits& typeTraits, rapidjson::Document::AllocatorType& allocator) {
        outputValue.SetObject();
        typeTraits.mForEach(anyValue.getInstance(), [&outputValue, &typeTraits, &allocator](const void* key, const void* value) {
            Any keyAny{ typeTraits.mKeyType, const_cast<void*>(key) };
            Any valueAny{ typeTraits.mValueType, const_cast<void*>(value) };

            rapidjson::Value keyValue;
            save(keyValue, keyAny, allocator);

            rapidjson::Value valueValue;
            save(valueValue, valueAny, allocator);

            outputValue.AddMember(keyValue, valueValue, allocator);
        });
    }

    void save(rapidjson::Value& outputValue, const Any& anyValue, rapidjson::Document::AllocatorType& allocator) {
        std::visit([&outputValue, &anyValue, &allocator](auto&& typeTraits) {
            _save(outputValue, anyValue, typeTraits, allocator);
        }, anyValue.getTypeId().getTypeIdInfo().getTypeTraits());
    }

	std::string save(const Any& anyValue) {
		const auto& reflectionContext{getCurrentReflectionContext()};
		if (!reflectionContext.hasClass(anyValue.getTypeId())) {
			printf("Class not registered.\n");
			return {};
		}

		const auto& classReflection{reflectionContext.getClass(anyValue.getTypeId())};
		const auto& className{classReflection.getName()};

		rapidjson::Document doc;
		auto& allocator{doc.GetAllocator()};

		rapidjson::Value classRootObject(rapidjson::kObjectType);
		classRootObject.SetObject();
		save(classRootObject, anyValue, allocator);

		rapidjson::Value classRootObjectKey(className.c_str(), allocator);
		doc.SetObject();
		doc.AddMember(classRootObjectKey, classRootObject, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter writer(buffer);
		doc.Accept(writer);

		return buffer.GetString();
	}

} // Core
