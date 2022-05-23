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

    void run(const std::string& textfile);

private:
    int processMacroDefinition(const std::string& textfile, int& index);    /* Returns an error code if one occurs */
    int processMacroCall(const std::string& textfile, int& index);  /* Returns an error code if one occurs */
    
    void produceOutputFile(const std::string& contents);

    MacroLibrary library_;
    std::string outputFileName_;
    std::string outputPath_;
};