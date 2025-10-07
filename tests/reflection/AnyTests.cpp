
#include <catch2/catch_test_macros.hpp>

import Ortha.RTTI.Any;
import Ortha.Core.TypeId;

TEST_CASE("AnyConstructDefault_GetTypeInstance_IsVoidTypeIdAndNullInstance", "Any") {
	using namespace Ortha::Core;
	Any any;

	const auto typeInstance = any.getTypeInstance();

	REQUIRE(typeInstance.getTypeId() == TypeId::get<void>());
	REQUIRE(typeInstance.getInstance() == nullptr);
}

TEST_CASE("AnyConstructWithIntTypeId_GetOwnsInstance_ReturnsTrue", "Any") {
	using namespace Ortha::Core;
	Any any(TypeId::get<int>());

	const bool ownsInstance = any.getOwnsInstance();

	REQUIRE(ownsInstance);
}

TEST_CASE("AnyConstructWithLValue_GetOwnsInstance_ReturnsFalse", "Any") {
	using namespace Ortha::Core;
	int instance{ 0 };
	Any any(instance);

	const bool ownsInstance = any.getOwnsInstance();

	REQUIRE(!ownsInstance);
}

TEST_CASE("AnyConstructWithInstanceOwnsInstanceTrue_DestroyAny_InstanceIsDestroyed") {
	using namespace Ortha::Core;
	bool isDestroyed = false;
	class TestClass {
	public:

		TestClass() = default;
		TestClass(bool& isDestroyed)
			: mIsDestroyed(&isDestroyed) {
		}

		~TestClass() {
			if (mIsDestroyed) {
				*mIsDestroyed = true;
			}
		}

		bool* mIsDestroyed{ nullptr };
	};
	TestClass* instance = new TestClass{ isDestroyed };

	{ Any any(TypeId::get<TestClass>(), instance, true); }

	REQUIRE(isDestroyed);
}

TEST_CASE("AnyConstructWithInstanceOwnsInstanceFalse_DestroyAny_InstanceIsNotDestroyed") {
	using namespace Ortha::Core;
	int* instance = new int{ 5 };

	{ Any any(TypeId::get<int>(), instance, false); }

	REQUIRE(*instance == 5);
}

TEST_CASE("AnyConstructWithIntTypeId_GetTypeInstance_IsIntTypeIdAndInstanceIsZero", "Any") {
	using namespace Ortha::Core;
	Any any(TypeId::get<int>());

	const auto typeInstance = any.getTypeInstance();

	REQUIRE(typeInstance.getTypeId() == TypeId::get<int>());
	REQUIRE(typeInstance.getInstance() != nullptr);
	int& intInstance = *static_cast<int*>(typeInstance.getInstance());
	REQUIRE(intInstance == 0);
}
