
#include <map>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

import Ortha.RTTI.Any;
import Core.ClassReflection;
import Core.JsonTypeSaverAdapter;
import Core.Log;
import Core.ReflectionContext;
import Core.TypeId;
import JsonTypeAdapterTestClasses;

TEST_CASE("ReflectedClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view expectedOutput = R"({
    "TestClass": {
        "stringProperty": "testString",
        "intProperty": 5,
        "doubleProperty": 10.0,
        "floatProperty": 15.0,
        "boolProperty": true
    }
})";
	Core::reflect<TestClass>(getCurrentReflectionContext());
	TestClass testClass{ "testString", 5, 10.0, 15.0f, true };
	Log log;

	const std::string output = save(log, Any{ testClass });

	REQUIRE(output == expectedOutput);
}


TEST_CASE("ReflectedClassWithNestedClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view expectedOutput = R"({
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
	auto& reflectionContext{ getCurrentReflectionContext() };
	reflect<TestClass>(reflectionContext);
	reflect<TestClass2>(reflectionContext);
	TestClass2 testClass{ "testString", 5, 10.0, 15.0f, true };
	Log log;

	const std::string output = save(log, Any{ testClass });

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithOptional_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view expectedOutput = R"({
    "TestClass3": {
        "optionalIntProperty": 5
    }
})";
	reflect<TestClass3>(getCurrentReflectionContext());
	TestClass3 testClass{};
	testClass.optionalIntProperty = 5;
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithMissingOptional_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view expectedOutput = R"({
    "TestClass3": {}
})";
	reflect<TestClass3>(getCurrentReflectionContext());
	TestClass3 testClass{};
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}


TEST_CASE("ReflectedClassWithOptionalClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view expectedOutput = R"({
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
	reflect<TestClass4>(getCurrentReflectionContext());
	TestClass4 testClass{};
	testClass.optionalClassProperty = { "testString", 5, 10.0, 15.0, true };
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithMissingOptionalClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view expectedOutput = R"({
    "TestClass4": {}
})";
	reflect<TestClass4>(getCurrentReflectionContext());
	TestClass4 testClass{};
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithVector_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view expectedOutput = R"({
    "VectorTestClass": {
        "vectorProperty": [
            5,
            10,
            15
        ]
    }
})";
	reflect<VectorTestClass>(getCurrentReflectionContext());
	VectorTestClass testClass{};
	testClass.vectorProperty = { 5, 10, 15 };
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithMap_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view expectedOutput = R"({
    "MapTestClass": {
        "mapProperty": {
            "a": 5,
            "b": 10,
            "c": 15
        }
    }
})";
	reflect<MapTestClass>(getCurrentReflectionContext());
	MapTestClass testClass{};
	testClass.mapProperty = { { "a", 5 }, { "b", 10 }, { "c", 15 } };
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}
