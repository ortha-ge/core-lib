
#include <map>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

import Core.Any;
import Core.ClassReflection;
import Core.JsonTypeLoaderAdapter;
import Core.Log;
import Core.ReflectionContext;
import Core.TypeId;
import JsonTypeAdapterTestClasses;

TEST_CASE("JsonInput_LoadReflectedClass_LoadedClassMatchesInput", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
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
	Log log;

	load(log, reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.stringProperty == "testString");
	REQUIRE(testClass.intProperty == 5);
	REQUIRE(testClass.doubleProperty == 10.0);
	REQUIRE(testClass.floatProperty == 15.0);
	REQUIRE(testClass.boolProperty == true);
}

TEST_CASE("JsonInput_LoadReflectedClassByName_InstanceIsCorrectType", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
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
	Log log;

	Any anyValue = load(log, reflectionContext, jsonInput);

	REQUIRE(TypeId::get<TestClass>() == anyValue.getTypeId());
	TestClass* testClass = static_cast<TestClass*>(anyValue.getInstance());
	REQUIRE(testClass->stringProperty == "testString");
	REQUIRE(testClass->intProperty == 5);
	REQUIRE(testClass->doubleProperty == 10.0);
	REQUIRE(testClass->floatProperty == 15.0);
	REQUIRE(testClass->boolProperty == true);
}

TEST_CASE("JsonInput_LoadReflectedClassWithNestedClass_LoadedClassMatchesInput", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
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
	Log log;

	load(log, reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.nestedProperty.stringProperty == "testString");
	REQUIRE(testClass.nestedProperty.intProperty == 5);
	REQUIRE(testClass.nestedProperty.doubleProperty == 10.0);
	REQUIRE(testClass.nestedProperty.floatProperty == 15.0);
	REQUIRE(testClass.nestedProperty.boolProperty == true);
}

TEST_CASE("JsonInput_LoadReflectedClassWithOptional_LoadedOptionalMatchesInput", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
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
	Log log;

	load(log, reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.optionalIntProperty == 5);
}

TEST_CASE("JsonInput_LoadReflectedClassWithMissingOptional_LoadedOptionalIsNull", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view jsonInput = R"({
	"TestClass3": {}
})";
	ReflectionContext reflectionContext{};
	reflect<TestClass3>(reflectionContext);
	TestClass3 testClass{};
	Any anyValue(testClass);
	Log log;

	load(log, reflectionContext, jsonInput, anyValue);

	REQUIRE(!testClass.optionalIntProperty.has_value());
}


TEST_CASE("JsonInput_LoadReflectedClassWithOptionalClass_LoadedOptionalMatchesInput", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
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
	Log log;

	load(log, reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.optionalClassProperty);
	const auto& innerProperty{ *testClass.optionalClassProperty };
	REQUIRE(testClass.optionalClassProperty->intProperty == 5);
}

TEST_CASE("JsonInput_LoadReflectedClassWithMissingOptionalClass_LoadedOptionalIsNull", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
	using namespace Core;
	constexpr std::string_view jsonInput = R"({
	"TestClass4": {}
})";
	ReflectionContext reflectionContext{};
	reflect<TestClass4>(reflectionContext);
	TestClass4 testClass{};
	Any anyValue(testClass);
	Log log;

	load(log, reflectionContext, jsonInput, anyValue);

	REQUIRE(!testClass.optionalClassProperty);
}

TEST_CASE("JsonInput_LoadReflectedClassWithVector_LoadedVectorMatchesInput", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
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
	Log log;

	load(log, reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.vectorProperty.size() == 3);
}

TEST_CASE("JsonInput_LoadReflectedClassWithMap_LoadedMapMatchesInput", "JsonTypeLoaderAdapterTests") {
	using namespace JsonTypeAdapterTestClasses;
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
	Log log;

	load(log, reflectionContext, jsonInput, anyValue);

	REQUIRE(testClass.mapProperty.size() == 3);
}
