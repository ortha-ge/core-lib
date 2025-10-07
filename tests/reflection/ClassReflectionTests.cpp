
#include <cstdint>

#include <catch2/catch_test_macros.hpp>

import Ortha.Core.ClassReflection;
import Ortha.Core.FileDescriptor;
import Ortha.Core.FileLoadRequest;
import Ortha.Core.FileLoadSystem;
import Ortha.Core.RawDataResource;
import Ortha.Core.ReflectionContext;
import Ortha.Core.TypeId;
import entt;

namespace ClassReflectionTests {

	class TestClass {
	public:
		int32_t publicProperty{};
		int32_t secondPublicProperty{};
	};

	class TestClass2 {
	public:
		bool bufferValue{};
		TestClass nestedClassProperty{};
	};

} // namespace ClassReflectionTests

TEST_CASE("ClassWithProperties_MemberOffset_ReturnsAddressMatchingOffset", "ClassReflection") {
	using namespace Core;
	using namespace ClassReflectionTests;

	auto firstOffset = memberOffset(&TestClass::publicProperty);
	auto secondOffset = memberOffset(&TestClass::secondPublicProperty);

	REQUIRE(firstOffset == 0);
	REQUIRE(secondOffset == sizeof(uint32_t));
}

TEST_CASE("ClassProperty_ApplyPropertyToInstance_AddressAndValueMatches", "ClassReflection") {
	using namespace Core;
	using namespace ClassReflectionTests;
	const TestClass classInstance{ 5, 10 };
	auto firstPropertyOffset = memberOffset(&TestClass::publicProperty);
	auto secondPropertyOffset = memberOffset(&TestClass::secondPublicProperty);
	const ClassProperty firstProperty(
		TypeId::get<int32_t>(), "publicProperty", firstPropertyOffset, sizeof(TestClass::publicProperty));
	const ClassProperty secondProperty(
		TypeId::get<int32_t>(), "secondPublicProperty", secondPropertyOffset, sizeof(TestClass::secondPublicProperty));

	const auto& firstPropertyValue{ firstProperty.apply<int32_t>(classInstance) };
	const auto& secondPropertyValue{ secondProperty.apply<int32_t>(classInstance) };

	REQUIRE(&firstPropertyValue == &classInstance.publicProperty);
	REQUIRE(&secondPropertyValue == &classInstance.secondPublicProperty);
	REQUIRE(firstPropertyValue == classInstance.publicProperty);
	REQUIRE(secondPropertyValue == classInstance.secondPublicProperty);
}

TEST_CASE("ClassWithNestedClassProperties_ApplyPropertyToInstance_AddressAndValueMatches", "ClassReflection") {
	using namespace Core;
	using namespace ClassReflectionTests;
	const TestClass2 classInstance{ false, 5, 10 };
	auto nestedClassOffset = memberOffset(&TestClass2::nestedClassProperty);
	auto firstPropertyOffset = nestedClassOffset + memberOffset(&TestClass::publicProperty);
	auto secondPropertyOffset = nestedClassOffset + memberOffset(&TestClass::secondPublicProperty);
	const ClassProperty nestedClassProperty(
		TypeId::get<TestClass2>(), "nestedClassProperty", nestedClassOffset, sizeof(TestClass2::nestedClassProperty));
	const ClassProperty firstProperty(
		TypeId::get<int32_t>(), "publicProperty", firstPropertyOffset, sizeof(TestClass::publicProperty));
	const ClassProperty secondProperty(
		TypeId::get<int32_t>(), "secondPublicProperty", secondPropertyOffset, sizeof(TestClass::secondPublicProperty));

	const auto& nestedPropertyValue{ nestedClassProperty.apply<TestClass>(classInstance) };
	const auto& firstPropertyValue{ firstProperty.apply<int32_t>(classInstance) };
	const auto& secondPropertyValue{ secondProperty.apply<int32_t>(classInstance) };

	REQUIRE(&nestedPropertyValue == &classInstance.nestedClassProperty);
	REQUIRE(&firstPropertyValue == &classInstance.nestedClassProperty.publicProperty);
	REQUIRE(&secondPropertyValue == &classInstance.nestedClassProperty.secondPublicProperty);
	REQUIRE(firstPropertyValue == classInstance.nestedClassProperty.publicProperty);
	REQUIRE(secondPropertyValue == classInstance.nestedClassProperty.secondPublicProperty);
}

TEST_CASE("ClassBuilderWithProperties_BuildAndGetProperty_OffsetAndSizesMatch", "ClassReflection") {
	using namespace Core;
	using namespace ClassReflectionTests;
	ReflectionContext reflectionContext;
	const auto classReflection = reflectionContext.addClass<TestClass>("TestClass")
									 .property("publicProperty", &TestClass::publicProperty)
									 .property("secondPublicProperty", &TestClass::secondPublicProperty)
									 .getReflection();

	REQUIRE(classReflection.hasProperty("publicProperty"));
	REQUIRE(classReflection.hasProperty("secondPublicProperty"));
	const auto& firstProperty = classReflection.getProperty("publicProperty");
	const auto& secondProperty = classReflection.getProperty("secondPublicProperty");
	REQUIRE(firstProperty.offset() == 0);
	REQUIRE(secondProperty.offset() == sizeof(uint32_t));
	REQUIRE(firstProperty.getTypeId().getTypeInfo().getSize() == sizeof(uint32_t));
	REQUIRE(secondProperty.getTypeId().getTypeInfo().getSize() == sizeof(uint32_t));
}

TEST_CASE("ClassBuilderWithProperties_BuildAndForEachProperties_EnumeratesProperties", "ClassReflection") {
	using namespace Core;
	using namespace ClassReflectionTests;
	ReflectionContext reflectionContext;
	const auto classReflection = reflectionContext.addClass<TestClass>("TestClass")
									 .property("publicProperty", &TestClass::publicProperty)
									 .property("secondPublicProperty", &TestClass::secondPublicProperty)
									 .getReflection();

	bool hasValidFirstProperty = false;
	bool hasValidSecondProperty = false;
	classReflection.forEachProperty([&](const ClassProperty& property) {
		if (property.getName() == "publicProperty") {
			hasValidFirstProperty = true;
		} else if (property.getName() == "secondPublicProperty") {
			hasValidSecondProperty = true;
		}
	});

	REQUIRE(hasValidFirstProperty);
	REQUIRE(hasValidSecondProperty);
}

TEST_CASE("ReflectionContext_AddClass_HasMatchingClass", "ClassReflection") {
	using namespace Core;
	using namespace ClassReflectionTests;
	ReflectionContext reflectionContext;
	reflectionContext.addClass<TestClass>("TestClass")
		.property("publicProperty", &TestClass::publicProperty)
		.property("secondPublicProperty", &TestClass::secondPublicProperty)
		.build();

	const bool hasClass = reflectionContext.hasClass<TestClass>();
	REQUIRE(hasClass);
	const auto& retrievedClassReflection = reflectionContext.getClass<TestClass>();
	REQUIRE(retrievedClassReflection.hasProperty("publicProperty"));
	REQUIRE(retrievedClassReflection.hasProperty("secondPublicProperty"));
}
