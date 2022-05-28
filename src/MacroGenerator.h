#pragma once
#include <iostream>
#include <string>

#include "MacroLibrary.h"

class MacroGenerator
{
    friend class TestModule;
    
public:
    MacroGenerator();
    ~MacroGenerator() = default;

    std::string process(const std::string& contents);

private:
    void processMacroDefinition(const std::string& contents, size_t& i);
    void processMacroCall(const std::string& contents, size_t& i, std::string& output);

    int processMacroParameterNumber(const std::string& contents, size_t& i, int nextParameterNumber);  /* Returns a number that is after a parameter delimiter & as an integer, -1 if there is an error */
    std::string processMacroArgument(const std::string& contents, size_t& i);

    void skipUntilClosingBracketOrEOF(const std::string& contents, size_t& i);
    void skipUntilClosingParenthesisOrEOF(const std::string& contents, size_t& i);
    
    void produceOutputFile(const std::string& contents);
    void createLogFile();
    void appendToLogFile(const std::string& message);

    MacroLibrary library_;
    std::string outputFileName_;
    std::string outputPath_;
    std::string logFileName_;
};