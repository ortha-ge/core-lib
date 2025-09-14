
#include <catch2/catch_test_macros.hpp>

import Core.Any;
import Core.TypeId;

TEST_CASE("AnyConstructDefault_GetTypeInstance_IsVoidTypeIdAndNullInstance", "Any") {
	using namespace Core;
	Any any;

	const auto typeInstance = any.getTypeInstance();

	REQUIRE(typeInstance.typeId == TypeId::get<void>());
	REQUIRE(typeInstance.instance == nullptr);
}

TEST_CASE("AnyConstructWithIntTypeId_GetOwnsInstance_ReturnsTrue", "Any") {
	using namespace Core;
	Any any(TypeId::get<int>());

	const bool ownsInstance = any.getOwnsInstance();

	REQUIRE(ownsInstance);
}

TEST_CASE("AnyConstructWithLValue_GetOwnsInstance_ReturnsFalse", "Any") {
	using namespace Core;
	int instance{ 0 };
	Any any(instance);

	const bool ownsInstance = any.getOwnsInstance();

	REQUIRE(!ownsInstance);
}

TEST_CASE("AnyConstructWithInstanceOwnsInstanceTrue_DestroyAny_InstanceIsDestroyed") {
	using namespace Core;
	int* instance = new int{ 5 };

	{ Any any(TypeId::get<int>(), instance, true); }

	REQUIRE(*instance != 5);
}

TEST_CASE("AnyConstructWithInstanceOwnsInstanceFalse_DestroyAny_InstanceIsNotDestroyed") {
	using namespace Core;
	int* instance = new int{ 5 };

	{ Any any(TypeId::get<int>(), instance, false); }

	REQUIRE(*instance == 5);
}

TEST_CASE("AnyConstructWithIntTypeId_GetTypeInstance_IsIntTypeIdAndInstanceIsZero", "Any") {
	using namespace Core;
	Any any(TypeId::get<int>());

	const auto typeInstance = any.getTypeInstance();

	REQUIRE(typeInstance.typeId == TypeId::get<int>());
	REQUIRE(typeInstance.instance != nullptr);
	int& intInstance = *static_cast<int*>(typeInstance.instance);
	REQUIRE(intInstance == 0);
}
