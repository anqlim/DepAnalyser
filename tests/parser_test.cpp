#include <gtest/gtest.h>
#include "../src/parser/cpp/cpp_parser.h"
#include "../src/parser/python/python_parser.h"
#include "../src/parser/java/java_parser.h"
#include "../src/parser/js/js_parser.h"
#include "../src/parser/go/go_parser.h"
using namespace DepAnalyser;

// CppParser
TEST(CppParserTest, FindsLocalIncludes) {
    Parsing::CppParser parser;
    auto deps = parser.parse("tests/fixtures/cpp/main.cpp", "tests/fixtures/cpp", "tests/fixtures/cpp");
    EXPECT_EQ(deps.size(), 2);
}

TEST(CppParserTest, IgnoresSystemIncludes) {
    Parsing::CppParser parser;
    auto deps = parser.parse("tests/fixtures/cpp/main.cpp", "tests/fixtures/cpp", "tests/fixtures/cpp");
    for (auto& dep : deps) {
        EXPECT_EQ(dep.find("vector"), std::string::npos);
    }
}

TEST(CppParserTest, ResolvesPath) {
    Parsing::CppParser parser;
    auto deps = parser.parse("tests/fixtures/cpp/main.cpp", "tests/fixtures/cpp", "tests/fixtures/cpp");
    bool found = false;
    for (auto& dep : deps) {
        if (dep.ends_with("utils.h")) found = true;
    }
    EXPECT_TRUE(found);
}

TEST(CppParserTest, EmptyFile) {
    Parsing::CppParser parser;
    auto deps = parser.parse("tests/fixtures/cpp/utils.h", "tests/fixtures/cpp", "tests/fixtures/cpp");
    EXPECT_TRUE(deps.empty());
}

TEST(CppParserTest, FileNotFound) {
    Parsing::CppParser parser;
    auto deps = parser.parse("tests/fixtures/cpp/nonexistent.cpp", "tests/fixtures/cpp", "tests/fixtures/cpp");
    EXPECT_TRUE(deps.empty());
}

// PythonParser
TEST(PythonParserTest, FindsRelativeImports) {
    Parsing::PythonParser parser;
    auto deps = parser.parse("tests/fixtures/python/main.py", "tests/fixtures/python", "tests/fixtures/python");
    EXPECT_EQ(deps.size(), 2);
}

TEST(PythonParserTest, IgnoresAbsoluteImports) {
    Parsing::PythonParser parser;
    auto deps = parser.parse("tests/fixtures/python/main.py", "tests/fixtures/python", "tests/fixtures/python");
    for (auto& dep : deps) {
        EXPECT_EQ(dep.find("os"), std::string::npos);
        EXPECT_EQ(dep.find("sys"), std::string::npos);
    }
}

TEST(PythonParserTest, FileNotFound) {
    Parsing::PythonParser parser;
    auto deps = parser.parse("tests/fixtures/python/nonexistent.py", "tests/fixtures/python", "tests/fixtures/python");
    EXPECT_TRUE(deps.empty());
}

// JavaParser
TEST(JavaParserTest, FindsImports) {
    Parsing::JavaParser parser;
    auto deps = parser.parse("tests/fixtures/java/Main.java", "tests/fixtures/java", "tests/fixtures/java");
    EXPECT_EQ(deps.size(), 2);
}

TEST(JavaParserTest, IgnoresStdLibImports) {
    Parsing::JavaParser parser;
    auto deps = parser.parse("tests/fixtures/java/Main.java", "tests/fixtures/java", "tests/fixtures/java");
    for (auto& dep : deps) {
        EXPECT_EQ(dep.find("java/util"), std::string::npos);
    }
}

TEST(JavaParserTest, FileNotFound) {
    Parsing::JavaParser parser;
    auto deps = parser.parse("tests/fixtures/java/Nonexistent.java", "tests/fixtures/java", "tests/fixtures/java");
    EXPECT_TRUE(deps.empty());
}

// JsParser
TEST(JsParserTest, FindsRelativeImports) {
    Parsing::JsParser parser;
    auto deps = parser.parse("tests/fixtures/js/main.js", "tests/fixtures/js", "tests/fixtures/js");
    EXPECT_EQ(deps.size(), 3);
}

TEST(JsParserTest, IgnoresNodeModules) {
    Parsing::JsParser parser;
    auto deps = parser.parse("tests/fixtures/js/main.js", "tests/fixtures/js", "tests/fixtures/js");
    for (auto& dep : deps) {
        EXPECT_EQ(dep.find("axios"), std::string::npos);
    }
}

TEST(JsParserTest, FileNotFound) {
    Parsing::JsParser parser;
    auto deps = parser.parse("tests/fixtures/js/nonexistent.js", "tests/fixtures/js", "tests/fixtures/js");
    EXPECT_TRUE(deps.empty());
}

// GoParser
TEST(GoParserTest, FindsRelativeImports) {
    Parsing::GoParser parser;
    auto deps = parser.parse("tests/fixtures/go/main.go", "tests/fixtures/go", "tests/fixtures/go");
    EXPECT_EQ(deps.size(), 2);
}

TEST(GoParserTest, IgnoresStdLibImports) {
    Parsing::GoParser parser;
    auto deps = parser.parse("tests/fixtures/go/main.go", "tests/fixtures/go", "tests/fixtures/go");
    for (auto& dep : deps) {
        EXPECT_EQ(dep.find("fmt"), std::string::npos);
        EXPECT_EQ(dep.find("os"), std::string::npos);
    }
}

TEST(GoParserTest, FileNotFound) {
    Parsing::GoParser parser;
    auto deps = parser.parse("tests/fixtures/go/nonexistent.go", "tests/fixtures/go", "tests/fixtures/go");
    EXPECT_TRUE(deps.empty());
}
