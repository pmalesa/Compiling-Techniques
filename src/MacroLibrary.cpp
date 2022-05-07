#include "MacroLibrary.h"

int MacroLibrary::add(const std::string& macroName, const std::string& macroBody, int nParams)
{
    /* Check if the number of parameters is at least two */
    if ( nParams < 2 )
        return -1;
    
    /* Check if macro body has a correct form */
    for ( std::size_t i = 0; i < macroBody.size(); ++i )
    {
        /* No nested definitions or calls are possible */
        if ( macroBody[i] == '#' || macroBody[i] == '$')    
            return -2; // nested definition or call detected
            
        if ( macroBody[i] == '&' )
        {
            /* Checking if the string after & delimeter is a valid number */
            if ( (i + 1 < macroBody.size() && !std::isdigit(macroBody[i + 1])) || i == macroBody.size() - 1)
                return -3; // no digit found after parameter delimeter '&'
            
            /* Obtaining the parameter number */
            std::string parameterNumberStr = "";
            for (std::size_t j = i + 1; j < macroBody.size() && std::isdigit(macroBody[j]); ++j, ++i )
                parameterNumberStr.push_back(macroBody[j]);

            /* Check if the given parameter number is valid */
            int parameterNumber = std::stoi(parameterNumberStr);
            if ( parameterNumber > nParams )
                return -4; // parameter number is not valid
        }
    }
    
    MacroDefinition md(macroName, macroBody, nParams);
    definitions_[macroName] = md;
    return 0;
}

std::pair<int, std::string> MacroLibrary::call(const std::string& macroName, const std::vector<std::string>& params)
{
    MacroDefinition& md = definitions_[macroName];
    
    if ( !exists(macroName) )
        return { -1, "" }; // macro does not exist
        
    if ( md.nParams > params.size() )
        return { -2, "" }; // too few arguments passed
        
    /* Processing passed arguments to the macro call */    
    std::vector<std::string> processedParams;
    if ( md.nParams < params.size() )
    {
        for ( std::size_t i = params.size() - md.nParams; i < params.size(); ++i)
            processedParams.push_back(params[i]);  
    }
    else
    {
        processedParams = params;    
    }
    
    /* Constructing the result of the macro call */   
    std::string result;
    for ( std::size_t i = 0; i < md.body.size(); ++i )
    {
        if ( md.body[i] == '&' )
        {
            /* Obtaining the parameter number */
            std::string parameterNumberStr = "";
            for (std::size_t j = i++ + 1; j < md.body.size() && std::isdigit(md.body[j]) ; ++j, ++i )
                parameterNumberStr.push_back(md.body[j]);

            int parameterNumber = std::stoi(parameterNumberStr);
            if ( parameterNumber == 0 )
                result.append(macroName);
            else                
                result.append(processedParams[parameterNumber - 1]);
        }
        
        result.push_back(md.body[i]);    
    }
    
    return { 0, result };    
}

bool MacroLibrary::exists(const std::string& macroName) const
{
    return definitions_.find(macroName) != definitions_.end();    
}

void MacroLibrary::clear()
{
    definitions_.clear();   
}