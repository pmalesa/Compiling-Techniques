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
    int processMacroDefinition(const std::string& textfile, int& index);    /* Returns an error code if one occurs */
    int processMacroCall(const std::string& textfile, int& index);  /* Returns an error code if one occurs */
    int processMacroParameter(const std::string& textfile, int& index); /* Returns -1 if the string until next white space is not a number */
    
    void produceOutputFile(const std::string& contents);

    MacroLibrary library_;
    std::string outputFileName_;
    std::string outputPath_;
};