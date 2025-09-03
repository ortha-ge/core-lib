
#include <map>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

import Core.Any;
import Core.ClassReflection;
import Core.JsonTypeLoaderAdapter;
import Core.ReflectionContext;
import Core.TypeId;

namespace JsonTypeLoaderAdapterTests {
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

}

template <>
void Core::reflect<JsonTypeLoaderAdapterTests::TestClass>(ReflectionContext& context) {
    using namespace JsonTypeLoaderAdapterTests;
    auto classReflection = ClassReflectionBuilder<TestClass>("TestClass")
        .property("stringProperty", &TestClass::stringProperty)
        .property("intProperty", &TestClass::intProperty)
        .property("doubleProperty", &TestClass::doubleProperty)
        .property("floatProperty", &TestClass::floatProperty)
        .property("boolProperty", &TestClass::boolProperty)
        .build();
    context.addClass(TypeId::get<TestClass>(), std::move(classReflection));
}

template <>
void Core::reflect<JsonTypeLoaderAdapterTests::TestClass2>(ReflectionContext& context) {
    using namespace JsonTypeLoaderAdapterTests;
    auto classReflection = ClassReflectionBuilder<TestClass2>("TestClass2")
        .property("nestedProperty", &TestClass2::nestedProperty)
        .build();
    context.addClass(TypeId::get<TestClass2>(), std::move(classReflection));
}

template <>
void Core::reflect<JsonTypeLoaderAdapterTests::TestClass3>(ReflectionContext& context) {
    using namespace JsonTypeLoaderAdapterTests;
    auto classReflection = ClassReflectionBuilder<TestClass3>("TestClass3")
        .property("optionalIntProperty", &TestClass3::optionalIntProperty)
        .build();
    context.addClass(TypeId::get<TestClass3>(), std::move(classReflection));
}

template <>
void Core::reflect<JsonTypeLoaderAdapterTests::TestClass4>(ReflectionContext& context) {
    using namespace JsonTypeLoaderAdapterTests;
    auto classReflection = ClassReflectionBuilder<TestClass4>("TestClass4")
        .property("optionalClassProperty", &TestClass4::optionalClassProperty)
        .build();
    context.addClass(TypeId::get<TestClass4>(), std::move(classReflection));
}

template <>
void Core::reflect<JsonTypeLoaderAdapterTests::VectorTestClass>(ReflectionContext& context) {
    using namespace JsonTypeLoaderAdapterTests;
    auto classReflection = ClassReflectionBuilder<VectorTestClass>("VectorTestClass")
        .property("vectorProperty", &VectorTestClass::vectorProperty)
        .build();
    context.addClass(TypeId::get<VectorTestClass>(), std::move(classReflection));
}

template <>
void Core::reflect<JsonTypeLoaderAdapterTests::MapTestClass>(ReflectionContext& context) {
    using namespace JsonTypeLoaderAdapterTests;
    auto classReflection = ClassReflectionBuilder<MapTestClass>("MapTestClass")
        .property("mapProperty", &MapTestClass::mapProperty)
        .build();
    context.addClass(TypeId::get<MapTestClass>(), std::move(classReflection));
}

TEST_CASE("JsonInput_LoadReflectedClass_LoadedClassMatchesInput", "JsonTypeLoaderAdapterTests") {
    using namespace JsonTypeLoaderAdapterTests;
    using namespace Core;
    constexpr std::string_view jsonInput = R"({
	"TestClass": {
	    "stringProperty": "testString",
	    "intProperty": 5,
	    "doubleProperty": 10.0,
	    "floatProperty": 15.0,
	    "boolProperty": true
	}
})";
	ReflectionContext reflectionContext{};
	reflect<TestClass>(reflectionContext);
	TestClass testClass{};
	Any anyValue(testClass);

	load(reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.stringProperty == "testString");
	REQUIRE(testClass.intProperty == 5);
	REQUIRE(testClass.doubleProperty == 10.0);
	REQUIRE(testClass.floatProperty == 15.0);
	REQUIRE(testClass.boolProperty == true);
}

TEST_CASE("JsonInput_LoadReflectedClassByName_InstanceIsCorrectType", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeLoaderAdapterTests;
	using namespace Core;
	constexpr std::string_view jsonInput = R"({
	"TestClass": {
	    "stringProperty": "testString",
	    "intProperty": 5,
	    "doubleProperty": 10.0,
	    "floatProperty": 15.0,
	    "boolProperty": true
	}
})";
	ReflectionContext reflectionContext{};
	reflect<TestClass>(reflectionContext);

	Any anyValue = load(reflectionContext, jsonInput);

	REQUIRE(TypeId::get<TestClass>() == anyValue.getTypeId());
	TestClass* testClass = static_cast<TestClass*>(anyValue.getInstance());
	REQUIRE(testClass->stringProperty == "testString");
	REQUIRE(testClass->intProperty == 5);
	REQUIRE(testClass->doubleProperty == 10.0);
	REQUIRE(testClass->floatProperty == 15.0);
	REQUIRE(testClass->boolProperty == true);
}

TEST_CASE("JsonInput_LoadReflectedClassWithNestedClass_LoadedClassMatchesInput", "JsonTypeLoaderAdapterTests") {
    using namespace JsonTypeLoaderAdapterTests;
    using namespace Core;
    constexpr std::string_view jsonInput = R"({
	"TestClass2": {
	    "nestedProperty": {
	        "stringProperty": "testString",
	        "intProperty": 5,
	        "doubleProperty": 10.0,
	        "floatProperty": 15.0,
	        "boolProperty": true
	    }
	}
})";
	ReflectionContext reflectionContext{};
	reflect<TestClass>(reflectionContext);
	reflect<TestClass2>(reflectionContext);
	TestClass2 testClass{};
	Any anyValue(testClass);

	load(reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.nestedProperty.stringProperty == "testString");
	REQUIRE(testClass.nestedProperty.intProperty == 5);
	REQUIRE(testClass.nestedProperty.doubleProperty == 10.0);
	REQUIRE(testClass.nestedProperty.floatProperty == 15.0);
	REQUIRE(testClass.nestedProperty.boolProperty == true);
}

TEST_CASE("JsonInput_LoadReflectedClassWithOptional_LoadedOptionalMatchesInput", "JsonTypeLoaderAdapterTests") {
    using namespace JsonTypeLoaderAdapterTests;
    using namespace Core;
    constexpr std::string_view jsonInput = R"({
	"TestClass3": {
	    "optionalIntProperty": 5
	}
})";
	ReflectionContext reflectionContext{};
	reflect<TestClass3>(reflectionContext);
	TestClass3 testClass{};
	Any anyValue(testClass);

	load(reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.optionalIntProperty == 5);
}

TEST_CASE("JsonInput_LoadReflectedClassWithMissingOptional_LoadedOptionalIsNull", "JsonTypeLoaderAdapterTests") {
    using namespace JsonTypeLoaderAdapterTests;
    using namespace Core;
    constexpr std::string_view jsonInput = R"({
	"TestClass3": {}
})";
	ReflectionContext reflectionContext{};
	reflect<TestClass3>(reflectionContext);
	TestClass3 testClass{};
	Any anyValue(testClass);

	load(reflectionContext, jsonInput, anyValue);

	REQUIRE(!testClass.optionalIntProperty.has_value());
}


TEST_CASE("JsonInput_LoadReflectedClassWithOptionalClass_LoadedOptionalMatchesInput", "JsonTypeLoaderAdapterTests") {
    using namespace JsonTypeLoaderAdapterTests;
    using namespace Core;
    constexpr std::string_view jsonInput = R"({
	"TestClass4": {
	    "optionalClassProperty": {
	        "stringProperty": "testString",
	        "intProperty": 5,
	        "doubleProperty": 10.0,
	        "floatProperty": 15.0,
	        "boolProperty": true
	    }
	}
})";
	ReflectionContext reflectionContext{};
	reflect<TestClass>(reflectionContext);
	reflect<TestClass4>(reflectionContext);
	TestClass4 testClass{};
	Any anyValue(testClass);

	load(reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.optionalClassProperty);
	const auto& innerProperty{*testClass.optionalClassProperty};
	REQUIRE(testClass.optionalClassProperty->intProperty == 5);
}

TEST_CASE("JsonInput_LoadReflectedClassWithMissingOptionalClass_LoadedOptionalIsNull", "JsonTypeLoaderAdapterTests") {
    using namespace JsonTypeLoaderAdapterTests;
    using namespace Core;
    constexpr std::string_view jsonInput = R"({
	"TestClass4": {}
})";
	ReflectionContext reflectionContext{};
	reflect<TestClass4>(reflectionContext);
	TestClass4 testClass{};
	Any anyValue(testClass);

	load(reflectionContext, jsonInput, anyValue);

	REQUIRE(!testClass.optionalClassProperty);
}

TEST_CASE("JsonInput_LoadReflectedClassWithVector_LoadedVectorMatchesInput", "JsonTypeLoaderAdapterTests") {
    using namespace JsonTypeLoaderAdapterTests;
    using namespace Core;
    constexpr std::string_view jsonInput = R"({
	"VectorTestClass": {
	    "vectorProperty": [
	        5,
	        10,
	        15
	    ]
	}
})";
	ReflectionContext reflectionContext{};
	reflect<VectorTestClass>(reflectionContext);
	VectorTestClass testClass{};
	Any anyValue(testClass);

	load(reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.vectorProperty.size() == 3);
}

TEST_CASE("JsonInput_LoadReflectedClassWithMap_LoadedMapMatchesInput", "JsonTypeLoaderAdapterTests") {
    using namespace JsonTypeLoaderAdapterTests;
    using namespace Core;
    constexpr std::string_view jsonInput = R"({
	"MapTestClass": {
	    "mapProperty": {
	        "a": 5,
	        "b": 10,
	        "c": 15
	    }
	}
})";
	ReflectionContext reflectionContext{};
	reflect<MapTestClass>(reflectionContext);
	MapTestClass testClass{};
	Any anyValue(testClass);

	load(reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.mapProperty.size() == 3);
}
