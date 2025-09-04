module;

#include <map>
#include <print>
#include <variant>

#include <rapidjson/document.h>

module Core.JsonTypeLoaderAdapter;

import Core.BasicTypeTraits;
import Core.ClassReflection;
import Core.MapTypeTraits;
import Core.OptionalTypeTraits;
import Core.ReflectionContext;
import Core.TypeId;
import Core.TypeTraits;
import Core.VectorTypeTraits;

namespace Core {

    template <typename... AllTypes>
    class AnyLoaderMapper {

        template <typename T>
        static bool _tryLoadAnyAsIntegral(const rapidjson::Value& inputValue, Any& anyValue) {

            constexpr auto min = std::numeric_limits<T>::min();
            constexpr auto max = std::numeric_limits<T>::max();

            T& value{ *static_cast<T*>(anyValue.getInstance()) };

            if constexpr (std::is_signed_v<T>) {
                auto intValue = inputValue.Get<int32_t>();
                if (intValue < min || intValue > max) {
                    printf("Exceeded range\n");
                    return true;
                }

                value = intValue;
            } else {
                auto uintValue = inputValue.Get<uint32_t>();
                if (uintValue < min || uintValue > max) {
                    printf("Exceeded range\n");
                    return true;
                }

                value = uintValue;
            }

            return true;
        }

        static bool _tryLoadAnyAsString(const rapidjson::Value& inputValue, Any& anyValue) {

            std::string& value{ *static_cast<std::string*>(anyValue.getInstance()) };
            value = inputValue.GetString();
            return true;
        }

        template <typename T>
        static bool _tryLoadAnyAs(const rapidjson::Value& inputValue, Any& anyValue) {
            if (anyValue.getTypeId() != TypeId::get<T>()) {
                return false;
            }

            if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
                return _tryLoadAnyAsIntegral<T>(inputValue, anyValue);
            } else if constexpr (std::is_same_v<T, std::string>) {
                return _tryLoadAnyAsString(inputValue, anyValue);
            } else {
                T& value{ *static_cast<T*>(anyValue.getInstance()) };
                value = inputValue.Get<T>();
            }

            return true;
        }

        template <typename Tail>
        static bool _tryLoadAny(const rapidjson::Value& inputValue, Any& anyValue) {
            return false;
        }

        template <typename Tail, typename T, typename... Types>
        static bool _tryLoadAny(const rapidjson::Value& inputValue, Any& anyValue) {
            return _tryLoadAnyAs<T>(inputValue, anyValue)
                || _tryLoadAny<Tail, Types...>(inputValue, anyValue);
        }

    public:

        static bool tryLoadAny(const rapidjson::Value& inputValue, Any& anyValue) {
            return _tryLoadAny<void, AllTypes...>(inputValue, anyValue);
        }

    };

    using AnyLoaderMapperTypes = AnyLoaderMapper<
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

    void load(const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue);

    template <typename T>
    void _load(const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue, const T&) {
        static_assert(false, "Unhandled type traits.");
    }

	void _load(const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue,
				   const VoidTypeTraits& typeTraits) {

    }

	void _load(const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue,
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
                        printf("Missing required property\n");
                    }

                    continue;
                }

                Any propertyAny{ property.getTypeId(), property.getRawPointer(classInstance) };
                load(reflectionContext, it->value, propertyAny);
            }
        } else if (AnyLoaderMapperTypes::tryLoadAny(inputValue, anyValue)) {

        } else {
            printf("Unhandled type\n");
        }
    }

    void _load(const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue, const OptionalTypeTraits& typeTraits) {
        Any wrappedTypeAny(typeTraits.elementType);
        load(reflectionContext, inputValue, wrappedTypeAny);
        anyValue = wrappedTypeAny;
    }

    void _load(const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue, const VectorTypeTraits& typeTraits) {
        if (!inputValue.IsArray()) {
            printf("Not an array.\n");
            return;
        }

        Any anyVector(TypeId::get<std::vector<Any>>());
        std::vector<Any>& wrappedVector = *static_cast<std::vector<Any>*>(anyVector.getInstance());
        wrappedVector.reserve(inputValue.Size());

        for (const auto& inputElement : inputValue.GetArray()) {
            Any wrappedTypeAny(typeTraits.elementType);
            load(reflectionContext, inputElement, wrappedTypeAny);
            wrappedVector.emplace_back(std::move(wrappedTypeAny));
        }

        anyValue = anyVector;
    }

    void _load(const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue, const MapTypeTraits& typeTraits) {
        if (!inputValue.IsObject()) {
            printf("Not an object.\n");
            return;
        }

        Any anyMap(TypeId::get<std::map<Any, Any>>());
        std::map<Any, Any>& wrappedMap = *static_cast<std::map<Any, Any>*>(anyMap.getInstance());

        for (const auto& objectElement : inputValue.GetObject()) {
            // Key
            Any wrappedKeyAny(TypeId::get<std::string>());
            std::string& wrappedKey{ *static_cast<std::string*>(wrappedKeyAny.getInstance())};
            wrappedKey = objectElement.name.GetString();

            // Value
            Any wrappedValueAny(typeTraits.valueType);
            load(reflectionContext, objectElement.value, wrappedValueAny);
            wrappedMap[wrappedKeyAny] = std::move(wrappedValueAny);
        }

        anyValue = anyMap;
    }

    void load(const ReflectionContext& reflectionContext, const rapidjson::Value& inputValue, Any& anyValue) {
        std::visit([&reflectionContext, &inputValue, &anyValue](auto&& typeTraits) {
            _load(reflectionContext, inputValue, anyValue, typeTraits);
        }, getTypeTraits(anyValue.getTypeId()));
    }

	void load(const ReflectionContext& reflectionContext, std::string_view jsonInput, Any& anyValue) {
		if (!reflectionContext.hasClass(anyValue.getTypeId())) {
			printf("Class not registered.\n");
			return;
		}

		const auto& classReflection{reflectionContext.getClass(anyValue.getTypeId())};
		const auto& className{classReflection.getName()};

		rapidjson::Document doc;
		doc.Parse(jsonInput.data(), jsonInput.length());

		auto classRootObjectIt = doc.FindMember(className.c_str());
		if (classRootObjectIt == doc.MemberEnd()) {
			printf("Couldn't find matching class root '%s' object.\n", className.c_str());
			return;
		}

		load(reflectionContext, classRootObjectIt->value, anyValue);
	}

	Any load(const ReflectionContext& reflectionContext, std::string_view jsonInput) {
		rapidjson::Document doc;
		doc.Parse(jsonInput.data(), jsonInput.length());

		if (doc.MemberCount() != 1) {
			printf("Requires a single root object with the name of the type represented.\n");
			return {};
		}

		auto rootClassObjectIt = doc.MemberBegin();
		const auto& rootClassNameValue{rootClassObjectIt->name};
		const char* rootClassName = rootClassNameValue.GetString();

		auto typeId = reflectionContext.getTypeIdByName(rootClassName);
		if (!typeId) {
			printf("TypeId not found with name '%s'.\n", rootClassName);
			return {};
		}

		if (!reflectionContext.hasClass(*typeId)) {
			printf("TypeId is not a registered class.\n");
			return {};
		}

		Any instance(*typeId);
		load(reflectionContext, rootClassObjectIt->value, instance);
		return instance;
	}

} // Core
