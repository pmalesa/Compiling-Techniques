#include <iostream>
#include <string>

#include "MacroLibrary.h"

class MacroGenerator
{
public:
    MacroGenerator() = default;

    void run(const std::string& textfile);

private:
    int processMacroDefinition(const std::string& textfile, int& index);    /* Returns an error code if one occurs */
    int processMacroCall(const std::string& textfile, int& index);  /* Returns an error code if one occurs */
    
    MacroLibrary library_;
};