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
import Core.VariantTypeTraits;
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

		std::visit([this](auto&& typeTraits) {
			using T = std::decay_t<decltype(typeTraits)>;
			if constexpr (!std::same_as<T, VoidTypeTraits>) {
				*this = typeTraits.constructFunc();
			}
		}, getTypeTraits(mTypeId));
	}

	Any::Any(TypeId typeId, void* instance)
		: Any(std::move(typeId), instance, false) {}

	Any::Any(TypeId typeId, const void* instance)
		: Any(std::move(typeId), instance, false) {}

	Any::Any(TypeId typeId, void* instance, bool ownsInstance)
		: mTypeId(std::move(typeId))
		, mInstance(instance)
		, mOwnsInstance(ownsInstance) {

	}

	Any::Any(TypeId typeId, const void* instance, bool ownsInstance)
		: Any(std::move(typeId), const_cast<void*>(instance), ownsInstance) {
	}

	Any::~Any() {
		if (mOwnsInstance) {
			std::visit([this](auto&& typeTraits) {
				using T = std::decay_t<decltype(typeTraits)>;
				if constexpr (!std::same_as<T, VoidTypeTraits>) {
					typeTraits.destroyFunc(*this);
				}
			}, getTypeTraits(mTypeId));

			mInstance = nullptr;
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

		lhsTypeTraits.optionalApplyFunc(lhs, rhs);
	}

	void _assign(
		Any& lhs, const OptionalTypeTraits& lhsTypeTraits, const Any& rhs, const OptionalTypeTraits& rhsTypeTraits) {
		assert(false);
		// if (lhsTypeTraits.mWrappedType != otherTypeId) {
		//     return *this;
		// }
		//
		// optionalTraits->mApply(getInstance(), other.getInstance());
	}

	void _assign(Any& lhs, const VariantTypeTraits& lhsTypeTraits, const Any& rhs, const BasicTypeTraits& rhsTypeTraits) {
		auto it = std::ranges::find(lhsTypeTraits.types, rhs.getTypeId());
		if (it == lhsTypeTraits.types.end()) {
			assert(false);
		}

		lhsTypeTraits.variantApplyFunc(lhs, rhs);
	}

	void
		_assign(Any& lhs, const VectorTypeTraits& lhsTypeTraits, const Any& rhs, const VectorTypeTraits& rhsTypeTraits) {
		// if (lhsTypeTraits.elementType != otherTypeId) {
		//     return *this;
		// }

		assert(rhs.getTypeId() == TypeId::get<std::vector<Any>>());

		const std::vector<Any>& anyVector{ *static_cast<const std::vector<Any>*>(rhs.getInstance()) };
		lhsTypeTraits.vectorApplyFunc(lhs, anyVector);
	}

	void _assign(Any& lhs, const MapTypeTraits& lhsTypeTraits, const Any& rhs, const MapTypeTraits& rhsTypeTraits) {
		assert((rhs.getTypeId() == TypeId::get<std::map<Any, Any>>()));

		const std::map<Any, Any>& anyMap{ *static_cast<const std::map<Any, Any>*>(rhs.getInstance()) };
		lhsTypeTraits.mapApplyFunc(lhs, anyMap);
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
