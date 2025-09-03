
#include <map>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

import Core.Any;
import Core.ClassReflection;
import Core.JsonTypeSaverAdapter;
import Core.ReflectionContext;
import Core.TypeId;

namespace JsonTypeSaverAdapterTests {
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
void Core::reflect<JsonTypeSaverAdapterTests::TestClass>(ReflectionContext& context) {
    using namespace JsonTypeSaverAdapterTests;
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
void Core::reflect<JsonTypeSaverAdapterTests::TestClass2>(ReflectionContext& context) {
    using namespace JsonTypeSaverAdapterTests;
    auto classReflection = ClassReflectionBuilder<TestClass2>("TestClass2")
        .property("nestedProperty", &TestClass2::nestedProperty)
        .build();
    context.addClass(TypeId::get<TestClass2>(), std::move(classReflection));
}

template <>
void Core::reflect<JsonTypeSaverAdapterTests::TestClass3>(ReflectionContext& context) {
    using namespace JsonTypeSaverAdapterTests;
    auto classReflection = ClassReflectionBuilder<TestClass3>("TestClass3")
        .property("optionalIntProperty", &TestClass3::optionalIntProperty)
        .build();
    context.addClass(TypeId::get<TestClass3>(), std::move(classReflection));
}

template <>
void Core::reflect<JsonTypeSaverAdapterTests::TestClass4>(ReflectionContext& context) {
    using namespace JsonTypeSaverAdapterTests;
    auto classReflection = ClassReflectionBuilder<TestClass4>("TestClass4")
        .property("optionalClassProperty", &TestClass4::optionalClassProperty)
        .build();
    context.addClass(TypeId::get<TestClass4>(), std::move(classReflection));
}

template <>
void Core::reflect<JsonTypeSaverAdapterTests::VectorTestClass>(ReflectionContext& context) {
    using namespace JsonTypeSaverAdapterTests;
    auto classReflection = ClassReflectionBuilder<VectorTestClass>("VectorTestClass")
        .property("vectorProperty", &VectorTestClass::vectorProperty)
        .build();
    context.addClass(TypeId::get<VectorTestClass>(), std::move(classReflection));
}

template <>
void Core::reflect<JsonTypeSaverAdapterTests::MapTestClass>(ReflectionContext& context) {
    using namespace JsonTypeSaverAdapterTests;
    auto classReflection = ClassReflectionBuilder<MapTestClass>("MapTestClass")
        .property("mapProperty", &MapTestClass::mapProperty)
        .build();
    context.addClass(TypeId::get<MapTestClass>(), std::move(classReflection));
}

TEST_CASE("ReflectedClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
    using namespace JsonTypeSaverAdapterTests;
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
    reflect<TestClass>(getCurrentReflectionContext());
    TestClass testClass{ "testString", 5, 10.0, 15.0f, true };

    const std::string output = save(Any{ testClass });

    REQUIRE(output == expectedOutput);
}


TEST_CASE("ReflectedClassWithNestedClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
    using namespace JsonTypeSaverAdapterTests;
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

    const std::string output = save(Any{ testClass });

    REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithOptional_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
    using namespace JsonTypeSaverAdapterTests;
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

    const std::string output = save(anyValue);

    REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithMissingOptional_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
    using namespace JsonTypeSaverAdapterTests;
    using namespace Core;
    constexpr std::string_view expectedOutput = R"({
    "TestClass3": {}
})";
    reflect<TestClass3>(getCurrentReflectionContext());
    TestClass3 testClass{};
    Any anyValue(testClass);

    const std::string output = save(anyValue);

    REQUIRE(output == expectedOutput);
}


TEST_CASE("ReflectedClassWithOptionalClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
    using namespace JsonTypeSaverAdapterTests;
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
    testClass.optionalClassProperty = {
        "testString",
        5,
        10.0,
        15.0,
        true
    };
    Any anyValue(testClass);

    const std::string output = save(anyValue);

    REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithMissingOptionalClass_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
    using namespace JsonTypeSaverAdapterTests;
    using namespace Core;
    constexpr std::string_view expectedOutput = R"({
    "TestClass4": {}
})";
    reflect<TestClass4>(getCurrentReflectionContext());
    TestClass4 testClass{};
    Any anyValue(testClass);

    const std::string output = save(anyValue);

    REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithVector_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
    using namespace JsonTypeSaverAdapterTests;
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
    testClass.vectorProperty = {
        5,
        10,
        15
    };
    Any anyValue(testClass);

    const std::string output = save(anyValue);

    REQUIRE(output == expectedOutput);
}

TEST_CASE("ReflectedClassWithMap_Save_OutputMatchesSourceClass", "JsonTypeSaverAdapterTests") {
    using namespace JsonTypeSaverAdapterTests;
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
    testClass.mapProperty = {
        { "a", 5 },
        { "b", 10 },
        { "c", 15 }
    };
    Any anyValue(testClass);

    const std::string output = save(anyValue);

    REQUIRE(output == expectedOutput);
}
