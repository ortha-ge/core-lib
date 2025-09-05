module;

#include <map>
#include <optional>
#include <string>
#include <vector>

export module JsonTypeAdapterTestClasses;

import Core.ClassReflection;
import Core.EnumReflection;
import Core.ReflectionContext;

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
void Core::reflect<JsonTypeAdapterTestClasses::TestClass>(ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<TestClass>("TestClass")
		.property("stringProperty", &TestClass::stringProperty)
		.property("intProperty", &TestClass::intProperty)
		.property("doubleProperty", &TestClass::doubleProperty)
		.property("floatProperty", &TestClass::floatProperty)
		.property("boolProperty", &TestClass::boolProperty)
		.build();
}

template<>
void Core::reflect<JsonTypeAdapterTestClasses::TestClass2>(ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<TestClass2>("TestClass2")
		.property("nestedProperty", &TestClass2::nestedProperty)
		.build();
}

template<>
void Core::reflect<JsonTypeAdapterTestClasses::TestClass3>(ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<TestClass3>("TestClass3")
		.property("optionalIntProperty", &TestClass3::optionalIntProperty)
		.build();
}

template<>
void Core::reflect<JsonTypeAdapterTestClasses::TestClass4>(ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<TestClass4>("TestClass4")
		.property("optionalClassProperty", &TestClass4::optionalClassProperty)
		.build();
}

template<>
void Core::reflect<JsonTypeAdapterTestClasses::VectorTestClass>(ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<VectorTestClass>("VectorTestClass")
		.property("vectorProperty", &VectorTestClass::vectorProperty)
		.build();
}

template<>
void Core::reflect<JsonTypeAdapterTestClasses::MapTestClass>(ReflectionContext& reflectionContext) {
	using namespace JsonTypeAdapterTestClasses;
	reflectionContext.addClass<MapTestClass>("MapTestClass")
		.property("mapProperty", &MapTestClass::mapProperty)
		.build();
}
