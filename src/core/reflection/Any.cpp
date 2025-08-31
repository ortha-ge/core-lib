module;

#include <cstdio>
#include <map>
#include <variant>
#include <vector>

module Core.Any;

import Core.TypeId;

namespace Core {

    Any::Any()
        : mTypeId{}
        , mInstance{ nullptr }
        , mOwnsInstance{ false } {
    }

    Any::Any(TypeId typeId)
        : mTypeId{ typeId }
        , mInstance(nullptr)
        , mOwnsInstance(true) {

        if (const auto* typeTraits = std::get_if<TypeTraits>(&mTypeId.getTypeIdInfo().getTypeTraits())) {
            mInstance = typeTraits->mConstruct();
        }
    }

    Any::Any(TypeId typeId, void* instance)
        : mTypeId(typeId)
        , mInstance(instance)
        , mOwnsInstance(false) {
    }

    Any::~Any() {
        if (mOwnsInstance) {
            if (const auto* typeTraits = std::get_if<TypeTraits>(&mTypeId.getTypeIdInfo().getTypeTraits())) {
                typeTraits->mDestruct(mInstance);
                mInstance = nullptr;
            }
        }
    }

    Any::Any(Any&& any) noexcept
        : mTypeId{ std::move(any.mTypeId) }
        , mInstance(any.mInstance)
        , mOwnsInstance(any.mOwnsInstance) {
        any.mInstance = nullptr;
        any.mOwnsInstance = false;
    }

    Any& Any::operator=(Any&& any) noexcept {
        mTypeId = std::move(any.mTypeId);
        mInstance = any.mInstance;
        mOwnsInstance = any.mOwnsInstance;

        any.mInstance = nullptr;
        any.mOwnsInstance = false;

        return *this;
    }

    const TypeId& Any::getTypeId() const {
        return mTypeId;
    }

    void* Any::getInstance() const {
        return mInstance;
    }

    template <typename LhsTraits, typename RhsTraits>
    void _assign(Any&, const LhsTraits&, const Any&, const RhsTraits&) {
        printf("Unhandled Any assignment.\n");
    }

    void _assign(Any& lhs, const Any& rhs) {
        const auto& rhsTypeId{ rhs.getTypeId() };
        const auto& rhsTypeIdInfo{ rhsTypeId.getTypeIdInfo() };
        const auto& rhsTypeTraits{ rhsTypeIdInfo.getTypeTraits() };
        if (const auto* typeTraits = std::get_if<TypeTraits>(&rhsTypeTraits)) {
            lhs = Any(rhs.getTypeId());
            typeTraits->mApply(lhs.getInstance(), rhs.getInstance());
        }
    }

    void _assign(Any& lhs, const TypeTraits& lhsTypeTraits, const Any& rhs, const TypeTraits& rhsTypeTraits) {
        printf("Break\n");
        // if (lhsTypeTraits.mWrappedType != otherTypeId) {
        //     return *this;
        // }
        //
        // optionalTraits->mApply(getInstance(), other.getInstance());
    }

    void _assign(Any& lhs, const OptionalTypeTraits& lhsTypeTraits, const Any& rhs, const TypeTraits& rhsTypeTraits) {
        if (lhsTypeTraits.mWrappedType != rhs.getTypeId()) {
            printf("Mismatched Any assignment to Optional Any\n");
            return;
        }

        lhsTypeTraits.mApply(lhs.getInstance(), rhs.getInstance());
    }

    void _assign(Any& lhs, const OptionalTypeTraits& lhsTypeTraits, const Any& rhs, const OptionalTypeTraits& rhsTypeTraits) {
        // if (lhsTypeTraits.mWrappedType != otherTypeId) {
        //     return *this;
        // }
        //
        // optionalTraits->mApply(getInstance(), other.getInstance());
    }

    void _assign(Any& lhs, const VectorTypeTraits& lhsTypeTraits, const Any& rhs, const TypeTraits& rhsTypeTraits) {
        // if (lhsTypeTraits.mWrappedType != otherTypeId) {
        //     return *this;
        // }

        if (rhs.getTypeId() != TypeId::get<std::vector<Any>>()) {
            printf("Vector assignment requires rhs of std::vector<Any>.\n");
            return;
        }

        const std::vector<Any>& anyVector{ *static_cast<const std::vector<Any>*>(rhs.getInstance()) };
        std::vector<void*> voidVector;
        voidVector.reserve(anyVector.size());

        for (const auto& anyElement : anyVector) {
            if (anyElement.getTypeId() != lhsTypeTraits.mWrappedType) {
                return;
            }

            voidVector.push_back(anyElement.getInstance());
        }

        lhsTypeTraits.mApply(lhs.getInstance(), voidVector);
    }

    void _assign(Any& lhs, const MapTypeTraits& lhsTypeTraits, const Any& rhs, const TypeTraits& rhsTypeTraits) {
        if (rhs.getTypeId() != TypeId::get<std::map<Any, Any>>()) {
            printf("Map assignment requires rhs of std::map<Any, Any>.\n");
            return;
        }

        const std::map<Any, Any>& anyMap{ *static_cast<const std::map<Any, Any>*>(rhs.getInstance()) };
        std::map<void*, void*> voidMap;

        for (const auto& [inputKey, inputValue] : anyMap) {
            if (inputKey.getTypeId() != lhsTypeTraits.mKeyType) {
                printf("Input key type must match output key type.\n");
                return;
            }

            if (inputValue.getTypeId() != lhsTypeTraits.mValueType) {
                printf("Input value type must match output value type.\n");
                return;
            }

            voidMap[inputKey.getInstance()] = inputValue.getInstance();
        }

        lhsTypeTraits.mApply(lhs.getInstance(), voidMap);
    }

    Any::Any(const Any& other) {
        if (getTypeId() == TypeId::get<void>()) {
            _assign(*this, other);
            return;
        }

        std::visit([this, &other](auto&& lhsTypeTraits) {
            std::visit([this, &lhsTypeTraits, &other](auto&& rhsTypeTraits) {
               _assign(*this, lhsTypeTraits, other, rhsTypeTraits);
            }, other.getTypeId().getTypeIdInfo().getTypeTraits());
        }, getTypeId().getTypeIdInfo().getTypeTraits());
    }

    Any& Any::operator=(const Any& other) {
        if (this == &other) {
            return *this;
        }

        if (getTypeId() == TypeId::get<void>()) {
            _assign(*this, other);
            return *this;
        }

        std::visit([this, &other](auto&& lhsTypeTraits) {
            std::visit([this, &lhsTypeTraits, &other](auto&& rhsTypeTraits) {
               _assign(*this, lhsTypeTraits, other, rhsTypeTraits);
            }, other.getTypeId().getTypeIdInfo().getTypeTraits());
        }, getTypeId().getTypeIdInfo().getTypeTraits());

        return *this;
    }

} // Core

