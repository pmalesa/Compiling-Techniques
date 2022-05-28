#pragma once
#include <queue>
#include <string>

class TestModule
{
public:
    ~TestModule() = default;

    static int runTests();

private:
    TestModule() = default;

    static void testMacroLibraryAdd();
    static void testMacroLibraryExists();
    static void testMacroLibraryClear();
    static void testMacroLibraryCall();

    static void testMacroGeneratorCallToNonDefinedMacro();
    static void testMacroGeneratorNumberOfParameters();
    static void testMacroGeneratorMacroDefinition();
    static void testMacroGeneratorMacroCall();
    static void testMacroGeneratorIncorrectMacroDefinition();
    static void testMacroGeneratorIncorrectMacroCall();
    static void testMacroGeneratorOnlyFreeText();
    static void testMacroGeneratorEmptyInputFile();

    static void clearErrorMessages();
    static void printErrorMessages();

    static std::queue<std::string> errorMessages_;

};


