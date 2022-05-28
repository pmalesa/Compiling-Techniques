#include "TestModule.h"
#include "MacroGenerator.h"
#include "MacroLibrary.h"

#include <iostream>
#include <cstring>

/* Initializing the static members */
std::queue<std::string> TestModule::errorMessages_;

int TestModule::runTests()
{
    clearErrorMessages();

    /* Running all tests */
    std::cout << "*----------------RUNNING-TESTS----------------*\n";
    testMacroLibraryAdd();
    testMacroLibraryExists();
    testMacroLibraryClear();
    testMacroLibraryCall();

    if (errorMessages_.empty())
        std::cout << "MacroLibrary tests passed.\n";
    else
        std::cout << "MacroLibrary tests failed.\n";

    testMacroGeneratorCallToNonDefinedMacro();
    testMacroGeneratorNumberOfParameters();
    testMacroGeneratorMacroDefinition();
    testMacroGeneratorMacroCall();
    testMacroGeneratorIncorrectMacroDefinition();
    testMacroGeneratorIncorrectMacroCall();
    testMacroGeneratorOnlyFreeText();
    testMacroGeneratorEmptyInputFile();

    if (errorMessages_.empty())
        std::cout << "MacroGenerator tests passed.\n";
    else
        std::cout << "MacroGenerator tests failed.\n";

    if (errorMessages_.empty())
    {
        std::cout << "All tests passed.\n";
        std::cout << "*------------------------------------------------------------------------------------------*\n";
        std::cout << "*------------------------------------------------------------------------------------------*\n\n";
        return 0;
    }
    else
    {
        std::cout << "Tests failed.\n";
        printErrorMessages();
        std::cout << "*------------------------------------------------------------------------------------------*\n";
        std::cout << "*------------------------------------------------------------------------------------------*\n\n";
        return -1;
    }
}

void TestModule::testMacroLibraryAdd()
{
    MacroLibrary ml;
    std::string macroName = "MACRO_1";
    std::string macroBody = "body_1 &1 body_2 &2";
    int nParams = 3;
    int result = ml.add(macroName, macroBody, nParams);
    MacroLibrary::MacroDefinition& definition = ml.definitions_["MACRO_1"];

    /* 1 */
    if (strcmp(definition.name.c_str(), "MACRO_1") != 0)
    {
        std::string message = "Failure in MacroLibrary::add(): Macro name saved incorrectly.";
        errorMessages_.push(message);
    }
    
    /* 2 */
    if (strcmp(definition.body.c_str(), "body_1 &1 body_2 &2") != 0)
    {
        std::string message = "Failure in MacroLibrary::add(): Macro body saved incorrectly.";
        errorMessages_.push(message);
    }

    /* 3 */
    if (definition.nParams != 3)
    {
        std::string message = "Failure in MacroLibrary::add(): Macro's number of parameters saved incorrectly.";
        errorMessages_.push(message);
    }

    /* 4 */
    macroName = "MACRO_2";
    macroBody = "body_1 &1 body_2 &2";
    nParams = 1;
    result = ml.add(macroName, macroBody, nParams);
    if (result != -1)
    {
        std::string message = "Failure in MacroLibrary::add(): Macro's number of parameters is fewer than two, and the proper error was not indicated.";
        errorMessages_.push(message);    
    }

    /* 5 */
    macroName = "MACRO_3";
    macroBody = "body_1 $MACRO_2(A, B) &1 body_2 &2";
    nParams = 3;
    result = ml.add(macroName, macroBody, nParams);
    if (result != -2)
    {
        std::string message = "Failure in MacroLibrary::add(): Nested macro call in a macro body, and the proper error was not indicated.";
        errorMessages_.push(message);    
    }

    /* 6 */
    macroName = "MACRO_4";
    macroBody = "body_1 #MACRO_0(&1, &2, &3){body_1 &1 body_2 &2} &1 body_2 &2";
    nParams = 3;
    result = ml.add(macroName, macroBody, nParams);
    if (result != -2)
    {
        std::string message = "Failure in MacroLibrary::add(): Nested macro definition in a macro body, and the proper error was not indicated.";
        errorMessages_.push(message);    
    }

    /* 7 */
    macroName = "MACRO_5";
    macroBody = "body_1 & body_2 &2";
    nParams = 3;
    result = ml.add(macroName, macroBody, nParams);
    if (result != -3)
    {
        std::string message = "Failure in MacroLibrary::add(): No number after parameter delimiter '&' in a macro body, and the proper error was not indicated.";
        errorMessages_.push(message);    
    }

    /* 8 */
    macroName = "MACRO_6";
    macroBody = "body_1 &4 body_2 &2";
    nParams = 3;
    result = ml.add(macroName, macroBody, nParams);
    if (result != -4)
    {
        std::string message = "Failure in MacroLibrary::add(): Number after parameter delimiter '&' in a macro body is greater than the total number of parameters, and the proper error was not indicated.";
        errorMessages_.push(message);    
    }
}

void TestModule::testMacroLibraryExists()
{
    MacroLibrary ml;
    std::string macroName = "MACRO_1";
    std::string macroBody = "body_1 &1 body_2 &2";
    int nParams = 3;
    ml.add(macroName, macroBody, nParams);   

    /* 1 */
    bool definitionExists = (ml.definitions_.find("MACRO_1") != ml.definitions_.end()); // definitionExists == true
    if (ml.exists("MACRO_1") != definitionExists)
    {
        std::string message = "Failure in MacroLibrary::exists(): Macro was added, but the exists() method returns false.";
        errorMessages_.push(message);
    }

    /* 2 */
    definitionExists = (ml.definitions_.find("MACRO_2") != ml.definitions_.end()); // definitionExists == false
    if (ml.exists("MACRO_2") != definitionExists)
    {
        std::string message = "Failure in MacroLibrary::exists(): Macro was not added, but the exists() method returns true.";
        errorMessages_.push(message);
    }
}

void TestModule::testMacroLibraryClear()
{
    MacroLibrary ml;
    std::string macroName = "MACRO_1";
    std::string macroBody = "body_1 &1 body_2 &2";
    int nParams = 3;
    ml.add(macroName, macroBody, nParams); 
    ml.clear();
    if (!ml.definitions_.empty())
    {
        std::string message = "Failure in MacroLibrary::clear(): clear() function was called, but the macro library is not empty.";
        errorMessages_.push(message);   
    }
}

void TestModule::testMacroLibraryCall()
{
    MacroLibrary ml;

    /* 1 */
    std::string macroName = "MACRO_1";
    std::string macroBody = "body_1 &1 body_2 &2";
    int nParams = 3;
    ml.add(macroName, macroBody, nParams);
    std::vector<std::string> params = {"param_1", "param_2", "param_3"};
    std::pair<int, std::string> result = ml.call("MACRO_1", params);
    if (result.first != 0 || strcmp(result.second.c_str(), "body_1 param_1 body_2 param_2") != 0)
    {
        std::string message = "Failure in MacroLibrary::call() (1): Macro call's result is different than expected.";
        errorMessages_.push(message); 
    }

    /* 2 */
    macroName = "MACRO_2";
    macroBody = "body_1 &1 body_2 &2";
    nParams = 3;
    ml.add(macroName, macroBody, nParams);
    params = {"param_1", "param_2", "param_3", "param_4"};
    result = ml.call("MACRO_2", params);
    if (result.first != 0 || strcmp(result.second.c_str(), "body_1 param_2 body_2 param_3") != 0)
    {
        std::string message = "Failure in MacroLibrary::call() (2): Macro call's result is different than expected.";
        errorMessages_.push(message); 
    }

    /* 3 */
    macroName = "MACRO_3";
    macroBody = "body_1 &1 body_2 &2";
    nParams = 2;
    ml.add(macroName, macroBody, nParams);
    params = {"param_1", "param_2", "param_3", "param_4", "param_5", "param_6", "param_7"};
    result = ml.call("MACRO_3", params);
    if (result.first != 0 || strcmp(result.second.c_str(), "body_1 param_6 body_2 param_7") != 0)
    {
        std::string message = "Failure in MacroLibrary::call() (3): Macro call's result is different than expected.";
        errorMessages_.push(message); 
    }

    /* 4 */
    params = {"param_1", "param_2"};
    result = ml.call("MACRO_4", params);
    if (result.first != -1 || strcmp(result.second.c_str(), "") != 0)
    {
        std::string message = "Failure in MacroLibrary::call(): Non-existent macro called, without an error.";
        errorMessages_.push(message); 
    }

    /* 5 */
    macroName = "MACRO_5";
    macroBody = "body_1 &1 body_2 &2";
    nParams = 4;
    ml.add(macroName, macroBody, nParams);
    params = {"param_1", "param_2"};
    result = ml.call("MACRO_5", params);
    if (result.first != -2 || strcmp(result.second.c_str(), "") != 0)
    {
        std::string message = "Failure in MacroLibrary::call(): Macro called with too few arguments and did not indicate error.";
        errorMessages_.push(message); 
    }
}

void TestModule::testMacroGeneratorCallToNonDefinedMacro()
{
    MacroGenerator mg;
    std::string input = "ABC$MACRO(X, Y, Z)DEF";
    std::string correct = "ABCDEF";
    std::string output = mg.process(input);
    if (strcmp(output.c_str(), correct.c_str()) != 0)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong output of macrogenerator's result when calling a macro that was not defined.";
        errorMessages_.push(message);   
    }
}

void TestModule::testMacroGeneratorNumberOfParameters()
{
    MacroGenerator mg;
    std::string input = "#MACRO(&1, &2, &3, &4){body_1 &1 body_2 &2}ABC$MACRO(X, Y, Z)DEF";
    std::string correct = "ABCDEF";
    std::string output = mg.process(input);
    if (strcmp(output.c_str(), correct.c_str()) != 0)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong output of macrogenerator's result when calling a macro with too few parameters.";
        errorMessages_.push(message);   
    }
}

void TestModule::testMacroGeneratorMacroDefinition()
{
    MacroGenerator mg;
    std::string input = "ABC#MACRO(&1, &2, &3){body_1 &1 body_2 &2}DEF";
    std::string correct = "ABCDEF";
    std::string output = mg.process(input);

    /* 1 */
    if (strcmp(output.c_str(), correct.c_str()) != 0)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong output of macrogenerator's result with a correctly defined macro.";
        errorMessages_.push(message);   
    }

    /* 2 */
    if (mg.library_.definitions_.find("MACRO") == mg.library_.definitions_.end())
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong macro name obtained from a correctly defined macro.";
        errorMessages_.push(message);   
    }

    MacroLibrary::MacroDefinition& md = mg.library_.definitions_["MACRO"];

    /* 3 */
    if (md.nParams != 3)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong number of parameters obtained from a correctly defined macro.";
        errorMessages_.push(message);   
    }

    /* 4 */
    if (strcmp(md.body.c_str(), "body_1 &1 body_2 &2") != 0)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong macro body obtained from a correctly defined macro.";
        errorMessages_.push(message);   
    }
}

void TestModule::testMacroGeneratorMacroCall()
{
    MacroGenerator mg;
    std::string input = "#MACRO(&1, &2, &3){body_1 &1 body_2 &2}ABC$MACRO(X, Y, Z)DEF";
    std::string correct = "ABCbody_1 X body_2 YDEF";
    std::string output = mg.process(input);
    if (strcmp(output.c_str(), correct.c_str()) != 0)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong output of macrogenerator's result with a correctly called macro.";
        errorMessages_.push(message);   
    }
}

void TestModule::testMacroGeneratorIncorrectMacroDefinition()
{
    MacroGenerator mg;
    std::string input = "ABC#MACRO(&1, &2, &3,DEF GHI";
    std::string correct = "ABC";
    std::string output = mg.process(input);
    if (strcmp(output.c_str(), correct.c_str()) != 0)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong output of macrogenerator's result with an incorrectly defined macro.";
        errorMessages_.push(message);   
    }
}

void TestModule::testMacroGeneratorIncorrectMacroCall()
{
    MacroGenerator mg;
    std::string input = "#MACRO(&1, &2, &3){body_1 &1 body_2 &2}ABC$MACROX, Y, Z)DEF";
    std::string correct = "ABCDEF";
    std::string output = mg.process(input);
    if (strcmp(output.c_str(), correct.c_str()) != 0)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong output of macrogenerator's result with an incorrectly called macro.";
        errorMessages_.push(message);   
    }
}

void TestModule::testMacroGeneratorOnlyFreeText()
{
    MacroGenerator mg;
    std::string input = "ABCDEF";
    std::string correct = "ABCDEF";
    std::string output = mg.process(input);
    if (strcmp(output.c_str(), correct.c_str()) != 0)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong output of macrogenerator's result when called on a file containing only free text.";
        errorMessages_.push(message);   
    }
}

void TestModule::testMacroGeneratorEmptyInputFile()
{
    MacroGenerator mg;
    std::string input = "";
    std::string correct = "";
    std::string output = mg.process(input);
    if (strcmp(output.c_str(), correct.c_str()) != 0)
    {
        std::string message = "Failure in MacroGenerator::process(): Wrong output of macrogenerator's result when called on an empty file.";
        errorMessages_.push(message);   
    }
}

void TestModule::clearErrorMessages()
{
    std::queue<std::string> empty;
    std::swap( errorMessages_, empty);
}

void TestModule::printErrorMessages()
{
    int i = 1;
    while (!errorMessages_.empty())
    {
        std::string& message = errorMessages_.front();
        std::cout << "  [" << i << "]: " << message << "\n";
        errorMessages_.pop();
        ++i;
    }
}