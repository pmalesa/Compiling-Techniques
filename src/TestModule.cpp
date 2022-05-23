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

    testMacroGeneratorEmptyInputFile();
    testMacroGeneratorIllDefinedMacroDefinition();
    testMacroGeneratorIllDefinedMacroCall();




    if (errorMessages_.empty())
    {
        std::cout << "All tests passed.\n";
        std::cout << "*---------------------------------------------*\n\n";
        return 0;
    }
    else
    {
        std::cout << "Tests failed.\n";
        printErrorMessages();
        std::cout << "*---------------------------------------------*\n\n";
        return -1;
    }
}

void TestModule::testMacroLibraryAdd()
{
    MacroLibrary ml;
    std::string macroName = "MACRO_1";
    std::string macroBody = "body_1 &1 body_2 &2";
    int nParams = 3;
    result ml.add(macroName, macroBody, nParams);
    MacroLibrary::MacroDefinition& definition = ml.definitions_["MACRO_1"];

    /* 1 */
    if (definition.name != "MACRO_1")
    {
        std::string message = "Failure in MacroLibrary::add(): Macro name saved incorrectly.";
        errorMessages_.push(message);
    }
    
    /* 2 */
    if (definition.body != "body_1 &1 body_2 &2")
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






    
}

void TestModule::testMacroLibraryExists()
{
    MacroLibrary ml;
    std::string macroName = "MACRO_1";
    std::string macroBody = "body_1 &1 body_2 &2";
    int nParams = 3;
    ml.add(macroName, macroBody, nParams);   

    bool definitionExists = (ml.definitions_.find("MACRO_1") != ml.definitions_.end()); // definitionExists == true
    if (ml.exists("MACRO_1") != definitionExists)
    {
        std::string message = "Failure in MacroLibrary::exists(): Macro was added, but the exists() method returns false.";
        errorMessages_.push(message);
    }

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

void TestModule::testMacroGeneratorEmptyInputFile()
{
    
}

void TestModule::testMacroGeneratorIllDefinedMacroDefinition()
{
    
}

void TestModule::testMacroGeneratorIllDefinedMacroCall()
{
    
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