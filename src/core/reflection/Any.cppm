module;

#include <functional>

export module Core.Any;

import Core.ReflectionContext;
import Core.TypeId;

export namespace Core {

    class Any {
    public:
		Any();
		Any(TypeId typeId, void* instance);
		Any(TypeId typeId, const void* instance);
		~Any();

		explicit Any(TypeId typeId);

		Any(const Any&);
		Any(Any&&) noexcept;

		Any& operator=(const Any& other);
		Any& operator=(Any&&) noexcept;

        template <typename T>
        explicit Any(T& instance)
            : mTypeId{ TypeId::get<T>() }
            , mInstance(&instance)
            , mOwnsInstance(false) {
        }

        const TypeId& getTypeId() const;
        void* getInstance() const;

    private:

        TypeId mTypeId{};
        void* mInstance{};
        bool mOwnsInstance{};

    };

} // Core

template<>
struct std::less<Core::Any>{
    bool operator() (const Core::Any& lhs, const Core::Any& rhs) const {
        return &lhs < &rhs;
    }
};
