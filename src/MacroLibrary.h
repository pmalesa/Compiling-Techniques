#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class MacroLibrary
{
    friend class TestModule;
    
private:
    class MacroDefinition
    {
    public:
        MacroDefinition()
        {
            name = "";
            body = "";
            nParams = 0;
        }
        
        MacroDefinition(const std::string& newName, const std::string& newBody, int newParamsCount)
        {
            name = newName;
            body = newBody;
            nParams = newParamsCount;
        }
    
        std::string name;
        std::string body;
        unsigned int nParams;
    };
    
public:
    MacroLibrary() = default;

    /* This method accepts only properly defined macros */
    int add(const std::string& macroName, const std::string& macroBody, int nParams);
    
    std::pair<int, std::string> call(const std::string& macroName, const std::vector<std::string>& params);
    
    bool exists(const std::string& macroName) const;
    
    void clear();
    
private:
    std::unordered_map<std::string, MacroDefinition> definitions_;
};