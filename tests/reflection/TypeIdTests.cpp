
#include <map>
#include <optional>
#include <variant>
#include <vector>

#include <catch2/catch_test_macros.hpp>

import Ortha.Core.TypeId;
import Ortha.Core.BasicTypeTraits;
import Ortha.Core.MapTypeTraits;
import Ortha.Core.OptionalTypeTraits;
import Ortha.Core.TypeTraits;
import Ortha.Core.VariantTypeTraits;
import Ortha.Core.VectorTypeTraits;

TEST_CASE("TwoTypeIdsFromIdenticalGetCalls_CheckAddressEquality_AddressesMatch", "TypeId") {
	using namespace Ortha::Core;
	const auto typeIdA = TypeId::get<void>();
	const auto typeIdB = TypeId::get<void>();

	const bool areAddressesEqual = &typeIdA.getTypeInfo() == &typeIdB.getTypeInfo();

	REQUIRE(areAddressesEqual);
}

TEST_CASE("TwoTypeIdsFromDifferentGetCalls_CheckAddressEquality_AddressesDoNotMatch", "TypeId") {
	using namespace Ortha::Core;
	const auto typeIdA = TypeId::get<int>();
	const auto typeIdB = TypeId::get<bool>();

	const bool areAddressesEqual = &typeIdA.getTypeInfo() == &typeIdB.getTypeInfo();

	REQUIRE(!areAddressesEqual);
}

TEST_CASE("TypeIdGetVoid_GetTypeTraits_ReturnsVoidTypeTraits", "TypeId") {
	using namespace Ortha::Core;
	const auto typeId = TypeId::get<void>();

	const auto& typeTraits = getTypeTraits(typeId);

	const bool isVoidTypeTraits = std::holds_alternative<VoidTypeTraits>(typeTraits);
	REQUIRE(isVoidTypeTraits);
}

TEST_CASE("TypeIdGetInt_GetTypeTraits_ReturnsBasicTypeTraits", "TypeId") {
	using namespace Ortha::Core;
	const auto typeId = TypeId::get<int>();

	const auto& typeTraits = getTypeTraits(typeId);

	const bool isBasicTypeTraits = std::holds_alternative<BasicTypeTraits>(typeTraits);
	REQUIRE(isBasicTypeTraits);
}

TEST_CASE("TypeIdGetMap_GetTypeTraits_ReturnsMapTypeTraits", "TypeId") {
	using namespace Ortha::Core;
	const auto typeId = TypeId::get<std::map<int, int>>();

	const auto& typeTraits = getTypeTraits(typeId);

	const bool isMapTypeTraits = std::holds_alternative<MapTypeTraits>(typeTraits);
	REQUIRE(isMapTypeTraits);
}

TEST_CASE("TypeIdGetOptional_GetTypeTraits_ReturnsOptionalTypeTraits", "TypeId") {
	using namespace Ortha::Core;
	const auto typeId = TypeId::get<std::optional<int>>();

	const auto& typeTraits = getTypeTraits(typeId);

	const bool isOptionalTypeTraits = std::holds_alternative<OptionalTypeTraits>(typeTraits);
	REQUIRE(isOptionalTypeTraits);
}

TEST_CASE("TypeIdGetVariant_GetTypeTraits_ReturnsVariantTypeTraits", "TypeId") {
	using namespace Ortha::Core;
	const auto typeId = TypeId::get<std::variant<int, bool>>();

	const auto& typeTraits = getTypeTraits(typeId);

	const bool isVariantTypeTraits = std::holds_alternative<VariantTypeTraits>(typeTraits);
	REQUIRE(isVariantTypeTraits);
}

TEST_CASE("TypeIdGetVector_GetTypeTraits_ReturnsVectorTypeTraits", "TypeId") {
	using namespace Ortha::Core;
	const auto typeId = TypeId::get<std::vector<int>>();

	const auto& typeTraits = getTypeTraits(typeId);

	const bool isVectorTypeTraits = std::holds_alternative<VectorTypeTraits>(typeTraits);
	REQUIRE(isVectorTypeTraits);
}
