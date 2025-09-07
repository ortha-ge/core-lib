module;

#include <cassert>
#include <cstdio>
#include <map>
#include <variant>
#include <vector>

module Core.Any;

import Core.TypeId;
import Core.TypeTraits;
import Core.BasicTypeTraits;
import Core.MapTypeTraits;
import Core.OptionalTypeTraits;
import Core.VectorTypeTraits;

namespace Core {

	Any::Any()
		: mTypeId{}
		, mInstance{ nullptr }
		, mOwnsInstance{ false } {}

	Any::Any(TypeId typeId)
		: mTypeId{ typeId }
		, mInstance(nullptr)
		, mOwnsInstance(true) {

		if (const auto* typeTraits = std::get_if<BasicTypeTraits>(&getTypeTraits(mTypeId))) {
			*this = typeTraits->constructFunc();
		}
	}

	Any::Any(TypeId typeId, void* instance)
		: mTypeId(typeId)
		, mInstance(instance)
		, mOwnsInstance(false) {}

	Any::Any(TypeId typeId, const void* instance)
		: Any(typeId, const_cast<void*>(instance)) {}

	Any::~Any() {
		if (mOwnsInstance) {
			if (const auto* typeTraits = std::get_if<BasicTypeTraits>(&getTypeTraits(mTypeId))) {
				typeTraits->destroyFunc(*this);
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

	const TypeId& Any::getTypeId() const { return mTypeId; }

	void* Any::getInstance() const { return mInstance; }

	template<typename LhsTraits, typename RhsTraits>
	void _assign(Any&, const LhsTraits& lhsTraits, const Any&, const RhsTraits& rhsTraits) {
		assert(false);
	}

	void _assign(Any& lhs, const Any& rhs) {
		const auto& rhsTypeTraits{ getTypeTraits(rhs.getTypeId()) };
		if (const auto* typeTraits = std::get_if<BasicTypeTraits>(&rhsTypeTraits)) {
			lhs = Any(rhs.getTypeId());
			typeTraits->applyFunc(lhs, rhs);
		}
	}

	void _assign(Any& lhs, const BasicTypeTraits& lhsTypeTraits, const Any& rhs, const BasicTypeTraits& rhsTypeTraits) {
		assert(lhsTypeTraits.typeId == rhsTypeTraits.typeId);

		lhsTypeTraits.applyFunc(lhs, rhs);
	}

	void
	_assign(Any& lhs, const OptionalTypeTraits& lhsTypeTraits, const Any& rhs, const BasicTypeTraits& rhsTypeTraits) {
		assert(lhsTypeTraits.elementType == rhs.getTypeId());

		lhsTypeTraits.applyFunc(lhs, rhs);
	}

	void _assign(
		Any& lhs, const OptionalTypeTraits& lhsTypeTraits, const Any& rhs, const OptionalTypeTraits& rhsTypeTraits) {
		// if (lhsTypeTraits.mWrappedType != otherTypeId) {
		//     return *this;
		// }
		//
		// optionalTraits->mApply(getInstance(), other.getInstance());
	}

	void
	_assign(Any& lhs, const VectorTypeTraits& lhsTypeTraits, const Any& rhs, const BasicTypeTraits& rhsTypeTraits) {
		// if (lhsTypeTraits.elementType != otherTypeId) {
		//     return *this;
		// }

		assert(rhs.getTypeId() == TypeId::get<std::vector<Any>>());

		const std::vector<Any>& anyVector{ *static_cast<const std::vector<Any>*>(rhs.getInstance()) };
		lhsTypeTraits.applyFunc(lhs, anyVector);
	}

	void _assign(Any& lhs, const MapTypeTraits& lhsTypeTraits, const Any& rhs, const BasicTypeTraits& rhsTypeTraits) {
		assert((rhs.getTypeId() == TypeId::get<std::map<Any, Any>>()));

		const std::map<Any, Any>& anyMap{ *static_cast<const std::map<Any, Any>*>(rhs.getInstance()) };
		lhsTypeTraits.applyFunc(lhs, anyMap);
	}

	Any::Any(const Any& other) {
		if (getTypeId() == TypeId::get<void>()) {
			_assign(*this, other);
			return;
		}

		std::visit(
			[this, &other](auto&& lhsTypeTraits) {
				std::visit(
					[this, &lhsTypeTraits, &other](auto&& rhsTypeTraits) {
						_assign(*this, lhsTypeTraits, other, rhsTypeTraits);
					},
					getTypeTraits(other.getTypeId()));
			},
			getTypeTraits(getTypeId()));
	}

	Any& Any::operator=(const Any& other) {
		if (this == &other) {
			return *this;
		}

		if (getTypeId() == TypeId::get<void>()) {
			_assign(*this, other);
			return *this;
		}

		std::visit(
			[this, &other](auto&& lhsTypeTraits) {
				std::visit(
					[this, &lhsTypeTraits, &other](auto&& rhsTypeTraits) {
						_assign(*this, lhsTypeTraits, other, rhsTypeTraits);
					},
					getTypeTraits(other.getTypeId()));
			},
			getTypeTraits(getTypeId()));

		return *this;
	}

} // namespace Core
