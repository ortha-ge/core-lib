module;

#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

export module JsonTypeAdapterTestClasses;

import Ortha.RTTI.Reflect;
import Ortha.RTTI.ReflectionContext;

export namespace JsonTypeAdapterTestClasses {
	class TestClass {
	public:
		std::string stringProperty;
		int intProperty{};
		double doubleProperty{};
		float floatProperty{};
		bool boolProperty{};
	};

	class TestClass2 {
	public:
		TestClass nestedProperty;
	};

	class TestClass3 {
	public:
		std::optional<int> optionalIntProperty{};
	};

	class TestClass4 {
	public:
		std::optional<TestClass> optionalClassProperty{};
	};

	class VariantTestClass {
	public:

		std::variant<int, std::string> variantProperty{};

	};

	class VectorTestClass {
	public:
		std::vector<int> vectorProperty{};
	};

	class MapTestClass {
	public:
		std::map<std::string, int> mapProperty{};
	};

} // namespace JsonTypeAdapterTestClasses

template<>
void Ortha::Core::reflect<JsonTypeAdapterTestClasses::TestClass>(RTTI::ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<TestClass>("TestClass")
		.field<&TestClass::stringProperty>("stringProperty")
		.field<&TestClass::intProperty>("intProperty")
		.field<&TestClass::doubleProperty>("doubleProperty")
		.field<&TestClass::floatProperty>("floatProperty")
		.field<&TestClass::boolProperty>("boolProperty");
}

template<>
void Ortha::Core::reflect<JsonTypeAdapterTestClasses::TestClass2>(RTTI::ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<TestClass2>("TestClass2")
		.field<&TestClass2::nestedProperty>("nestedProperty");
}

template<>
void Ortha::Core::reflect<JsonTypeAdapterTestClasses::TestClass3>(RTTI::ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<TestClass3>("TestClass3")
		.field<&TestClass3::optionalIntProperty>("optionalIntProperty");
}

template<>
void Ortha::Core::reflect<JsonTypeAdapterTestClasses::TestClass4>(RTTI::ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<TestClass4>("TestClass4")
		.field<&TestClass4::optionalClassProperty>("optionalClassProperty");
}

template <>
void Ortha::Core::reflect<JsonTypeAdapterTestClasses::VariantTestClass>(RTTI::ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<VariantTestClass>("VariantTestClass")
		.field<&VariantTestClass::variantProperty>("variantProperty");
}

template<>
void Ortha::Core::reflect<JsonTypeAdapterTestClasses::VectorTestClass>(RTTI::ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<VectorTestClass>("VectorTestClass")
		.field<&VectorTestClass::vectorProperty>("vectorProperty");
}

template<>
void Ortha::Core::reflect<JsonTypeAdapterTestClasses::MapTestClass>(RTTI::ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<MapTestClass>("MapTestClass")
		.field<&MapTestClass::mapProperty>("mapProperty");
}
