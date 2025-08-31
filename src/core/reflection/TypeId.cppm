module;

#include <functional>
#include <map>
#include <optional>
#include <variant>
#include <vector>

export module Core.TypeId;

export namespace Core {
    class TypeId;
}

template <>
struct std::hash<Core::TypeId> {
    size_t operator()(const Core::TypeId& typeId) const noexcept;
};

export namespace Core {

    class TypeIdInfo;

    class TypeId {
    public:

        TypeId();
        TypeId(const TypeId&);
        TypeId(TypeId&&) noexcept;
        ~TypeId();

        TypeId& operator=(const TypeId&);
        TypeId& operator=(TypeId&&) noexcept;

        bool operator==(const TypeId&) const;

        template <typename T, std::enable_if_t<std::is_void_v<T>, bool> = true>
        static TypeId get();

        template <typename T, std::enable_if_t<!std::is_void_v<T>, bool> = true>
        static TypeId get();

        // TODO: This should be automatically deduced from any call to 'get<T>()'
        template <typename T, typename Traits>
        static TypeId getWithTypeTraits(Traits&& typeTraits);

        const TypeIdInfo& getTypeIdInfo() const;

    private:

        TypeId(const TypeIdInfo& typeIdInfo);

        const TypeIdInfo* mTypeIdInfo{};

    };

    class TypeTraits {
    public:

        using ConstructFunction = std::function<void*()>;
        using DestructFunction = std::function<void(void*)>;
        using ApplyFunction = std::function<void(void*, const void*)>;

        ConstructFunction mConstruct{};
        DestructFunction mDestruct{};
        ApplyFunction mApply{};
    };

    class OptionalTypeTraits {
    public:

        using ApplyFunction = std::function<void(void*, const void*)>;
        using GetFunction = std::function<void*(void*)>;

        TypeId mWrappedType{};
        ApplyFunction mApply{};
        GetFunction mGet{};

    };

    class VectorTypeTraits {
    public:

        using ApplyFunction = std::function<void(void*, const std::vector<void*>&)>;
        using ForEachFunction = std::function<void(void*, std::function<void(const void*)>)>;

        TypeId mWrappedType{};
        ApplyFunction mApply{};
        ForEachFunction mForEach{};
    };

    class MapTypeTraits {
    public:

        using ApplyFunction = std::function<void(void*, const std::map<void*, void*>&)>;
        using ForEachFunction = std::function<void(void*, std::function<void(const void*, const void*)>)>;

        TypeId mKeyType{};
        TypeId mValueType{};
        ApplyFunction mApply{};
        ForEachFunction mForEach{};
    };

    using AllTypeTraits = std::variant<TypeTraits, OptionalTypeTraits, VectorTypeTraits, MapTypeTraits>;

    class TypeIdInfo {
    public:

        explicit TypeIdInfo(size_t size);
        TypeIdInfo(AllTypeTraits traits, size_t size);
        ~TypeIdInfo() = default;

        TypeIdInfo(const TypeIdInfo&) = delete;
        TypeIdInfo& operator=(const TypeIdInfo&) = delete;
        TypeIdInfo(TypeIdInfo&&) noexcept = delete;
        TypeIdInfo& operator=(TypeIdInfo&&) noexcept = delete;

        const AllTypeTraits& getTypeTraits() const;
        [[nodiscard]] size_t getSize() const;

    private:

        AllTypeTraits mTypeTraits{};
        size_t mSize;

    };

    template <class T, typename MemberType>
    TypeId memberTypeId(MemberType T::* property) {
        return TypeId::get<MemberType>();
    }

    template <class T, typename MemberType>
    TypeId memberTypeId(std::optional<MemberType> T::* property) {
        auto applyFunc = [](void* optionalInstance, const void* wrappedTypeInstance) {
            auto& optional = *static_cast<std::optional<MemberType>*>(optionalInstance);
            auto& wrappedType = *static_cast<const MemberType*>(wrappedTypeInstance);
            optional = wrappedType;
        };

        auto getFunc = [](void* optionalInstance) -> void* {
            auto& optional = *static_cast<std::optional<MemberType>*>(optionalInstance);
            return optional.has_value() ? &optional.value() : nullptr;
        };

        OptionalTypeTraits optionalTraits(
            TypeId::get<MemberType>(),
            std::move(applyFunc),
            std::move(getFunc)
        );

        return TypeId::getWithTypeTraits<std::optional<MemberType>>(std::move(optionalTraits));
    }

    template <class T, typename MemberType>
    TypeId memberTypeId(std::vector<MemberType> T::* property) {
        auto applyFunc = [](void* vectorInstance, const std::vector<void*>& inputVector) {
            auto& vector = *static_cast<std::vector<MemberType>*>(vectorInstance);
            vector = {};

            for (const auto& inputElement : inputVector) {
                auto& wrappedType = *static_cast<const MemberType*>(inputElement);
                vector.emplace_back(wrappedType);
            }
        };

        auto forEachFunc = [](void* vectorInstance, std::function<void(const void*)> visitor) {
            auto& vector = *static_cast<std::vector<MemberType>*>(vectorInstance);
            for (const auto& outputElement : vector) {
                visitor(&outputElement);
            }
        };

        VectorTypeTraits vectorTraits(
            TypeId::get<MemberType>(),
            std::move(applyFunc),
            std::move(forEachFunc)
        );

        return TypeId::getWithTypeTraits<std::vector<MemberType>>(std::move(vectorTraits));
    }

    template <class T, typename KeyType, typename ValueType>
    TypeId memberTypeId(std::map<KeyType, ValueType> T::* property) {
        auto applyFunc = [](void* mapInstance, const std::map<void*, void*>& inputMap) {
            auto& map = *static_cast<std::map<KeyType, ValueType>*>(mapInstance);
            map = {};

            for (const auto& [inputKey, inputValue] : inputMap) {
                auto& wrappedKeyType = *static_cast<const KeyType*>(inputKey);
                auto& wrappedValueType = *static_cast<const ValueType*>(inputValue);
                map[wrappedKeyType] = wrappedValueType;
            }
        };

        auto forEachFunc = [](void* mapInstance, std::function<void(const void*, const void*)> visitor) {
            auto& map = *static_cast<std::map<KeyType, ValueType>*>(mapInstance);
            for (const auto& [outputKey, outputValue] : map) {
                visitor(&outputKey, &outputValue);
            }
        };

        MapTypeTraits mapTraits(
            TypeId::get<KeyType>(),
            TypeId::get<ValueType>(),
            std::move(applyFunc),
            std::move(forEachFunc)
        );

        return TypeId::getWithTypeTraits<std::map<KeyType, ValueType>>(std::move(mapTraits));
    }

    template <typename T, std::enable_if_t<std::is_void_v<T>, bool>>
    TypeId TypeId::get() {
        static TypeIdInfo typeIdInfo{ 0zu };
        return { typeIdInfo };
    }

    template <typename T, std::enable_if_t<!std::is_void_v<T>, bool>>
    TypeId TypeId::get() {
        auto constructFunc = []() -> void* {
            return new T{};
        };

        auto destructFunc = [](void* instance) {
            delete static_cast<T*>(instance);
        };

        auto applyFunc = [](void* destInstance, const void* sourceInstance) {
            T& dest{ *static_cast<T*>(destInstance)};
            dest = *static_cast<const T*>(sourceInstance);
        };

        TypeTraits typeTraits {
            std::move(constructFunc),
            std::move(destructFunc),
            std::move(applyFunc),
        };
        static TypeIdInfo typeIdInfo{ std::move(typeTraits), sizeof(T) };
        return { typeIdInfo };
    }

    template <typename T, typename Traits>
    TypeId TypeId::getWithTypeTraits(Traits&& typeTraits) {
        static TypeIdInfo typeIdInfo{ typeTraits, sizeof(T) };
        return { typeIdInfo };
    }

} // Core
