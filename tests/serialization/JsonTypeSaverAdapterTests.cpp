
#include <map>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

import Ortha.Core.JsonTypeSaverAdapter;
import Ortha.Core.Log;
import Ortha.RTTI.Reflect;
import Ortha.RTTI.Any;
import Ortha.RTTI.ReflectionContextStack;
import JsonTypeAdapterTestClasses;

TEST_CASE("ReflectedClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Ortha::Core;
	using namespace Ortha::RTTI;
	constexpr std::string_view expectedOutput = R"({
    "TestClass": {
        "stringProperty": "testString",
        "intProperty": 5,
        "doubleProperty": 10.0,
        "floatProperty": 15.0,
        "boolProperty": true
    }
})";
	reflect<TestClass>(ReflectionContextStack::getCurrentContext());
	TestClass testClass{ "testString", 5, 10.0, 15.0f, true };
	Log log;

	const std::string output = save(log, Any{ testClass });

	REQUIRE(output == expectedOutput);
}


TEST_CASE("ReflectedClassWithNestedClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Ortha::Core;
	using namespace Ortha::RTTI;
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
	auto& reflectionContext{ ReflectionContextStack::getCurrentContext() };
	reflect<TestClass>(reflectionContext);
	reflect<TestClass2>(reflectionContext);
	TestClass2 testClass{ "testString", 5, 10.0, 15.0f, true };
	Log log;

	const std::string output = save(log, Any{ testClass });

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithOptional_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Ortha::Core;
	using namespace Ortha::RTTI;
	constexpr std::string_view expectedOutput = R"({
    "TestClass3": {
        "optionalIntProperty": 5
    }
})";
	reflect<TestClass3>(ReflectionContextStack::getCurrentContext());
	TestClass3 testClass{};
	testClass.optionalIntProperty = 5;
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithMissingOptional_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Ortha::Core;
	using namespace Ortha::RTTI;
	constexpr std::string_view expectedOutput = R"({
    "TestClass3": {}
})";
	reflect<TestClass3>(ReflectionContextStack::getCurrentContext());
	TestClass3 testClass{};
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}


TEST_CASE("ReflectedClassWithOptionalClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Ortha::Core;
	using namespace Ortha::RTTI;
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
	reflect<TestClass4>(ReflectionContextStack::getCurrentContext());
	TestClass4 testClass{};
	testClass.optionalClassProperty = { "testString", 5, 10.0, 15.0, true };
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithMissingOptionalClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Ortha::Core;
	using namespace Ortha::RTTI;
	constexpr std::string_view expectedOutput = R"({
    "TestClass4": {}
})";
	reflect<TestClass4>(ReflectionContextStack::getCurrentContext());
	TestClass4 testClass{};
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithVector_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Ortha::Core;
	using namespace Ortha::RTTI;
	constexpr std::string_view expectedOutput = R"({
    "VectorTestClass": {
        "vectorProperty": [
            5,
            10,
            15
        ]
    }
})";
	reflect<VectorTestClass>(ReflectionContextStack::getCurrentContext());
	VectorTestClass testClass{};
	testClass.vectorProperty = { 5, 10, 15 };
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithMap_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Ortha::Core;
	using namespace Ortha::RTTI;
	constexpr std::string_view expectedOutput = R"({
    "MapTestClass": {
        "mapProperty": {
            "a": 5,
            "b": 10,
            "c": 15
        }
    }
})";
	reflect<MapTestClass>(ReflectionContextStack::getCurrentContext());
	MapTestClass testClass{};
	testClass.mapProperty = { { "a", 5 }, { "b", 10 }, { "c", 15 } };
	Any anyValue(testClass);
	Log log;

	const std::string output = save(log, anyValue);

	REQUIRE(output == expectedOutput);
}
