#include "MacroGenerator.h"

#include <filesystem>
#include <fstream>
#include <cstring>

MacroGenerator::MacroGenerator() : outputFileName_("output.txt"), outputPath_("../output"), logFileName_("log")
{

}

std::string MacroGenerator::process(const std::string& contents)
{
    std::string output;
    output.reserve(512);
    
    /* Parsing the input text file */
    std::size_t i = 0;
    while (i < contents.size())
    {   
        if (contents[i] == '#') // Add the found macro definition into the library
        {
            processMacroDefinition(contents, ++i);
        }
        else if (contents[i] == '$') // Search of the macro in the library
        {
            processMacroCall(contents, ++i, output);
        }
        else // Copy free text to the output
        {
            output += contents[i];    
            ++i;
        }
    }
    produceOutputFile(output);

    return output;
}

void MacroGenerator::processMacroDefinition(const std::string& contents, size_t& i)
{
    if (i > contents.size() - 1)
        return;

    int initial = i;
    size_t j = i;
    /* Checking if there is a whitespace between a macro definition delimiter and the macro name or a macro delimiter is at the end of the file */
    if ((j < contents.size() && std::isspace(contents[j])) || j >= contents.size())
    {
        std::cout << "[ERROR] Found incorrect macro definition at position " << initial - 1 << ".\n";
        skipUntilClosingBracketOrEOF(contents, i);
        return;
    }
    
    std::string name = "";
    std::string body = "";
    int nParams = 0;
    
    /* Obtaining the name of the macro */
    while (j < contents.size())
    {
        /* Whitespace in macro name found */
        if (std::isspace(contents[j]))
        {
            std::cout << "[ERROR] Found incorrect macro definition at position " << initial - 1 << ".\n";
            skipUntilClosingBracketOrEOF(contents, i);
            return;
        }

        /* Opening parenthesis not found */
        if (j == contents.size() - 1 && contents[j] != '(')
        {
            std::cout << "[ERROR] Found incorrect macro definition at position " << initial - 1 << ".\n";
            skipUntilClosingBracketOrEOF(contents, i);
            return;    
        }   

        /* Opening parenthesis (parameter block) found */
        if (contents[j] == '(')
        {
            if (name.empty())
            {
                std::cout << "[ERROR] Found macro definition without a specified macro name at position " << initial - 1 << ".\n";
                skipUntilClosingBracketOrEOF(contents, i);
                return;
            } 
            else
            {
                ++j;
                break;
            }
        }

        name.push_back(contents[j]);
        ++j;
    }
    
    /* Obtaining number of parameters of the macro */
    int nextParameterNumber = 1;
    while (j < contents.size())
    {
        if (std::isspace(contents[j]))
        {
            ++j;
            continue;
        }

        if (contents[j] != '&')
        {
            std::cout << "[ERROR] Found macro definition with incorrectly defined parameters at position " << initial - 1 << ".\n";
            skipUntilClosingBracketOrEOF(contents, i);
            return;
        }

        ++j;
        if (j > contents.size() - 1 || !std::isdigit(contents[j]))
        {
            std::cout << "[ERROR] Found macro definition with incorrectly defined parameters at position " << initial - 1 << ".\n";
            skipUntilClosingBracketOrEOF(contents, i);
            return; 
        }

        /* Obtaining the found parameter number */
        int obtained_number = processMacroParameterNumber(contents, j, nextParameterNumber);
        if (obtained_number == -1)
        {
            std::cout << "[ERROR] Found macro definition with wrong parameter number at position " << initial - 1 << ".\n";
            skipUntilClosingBracketOrEOF(contents, i);
            return;
        }
        else if (obtained_number == -2)
        {
            std::cout << "[ERROR] Found macro definition with a parameter number containing non-digit characters at position " << initial - 1 << ".\n";
            skipUntilClosingBracketOrEOF(contents, i);
            return;
        }
        else if (obtained_number == -3)
        {
            std::cout << "[ERROR] Found incorrect macro definition at position " << initial - 1 << ".\n";
            skipUntilClosingBracketOrEOF(contents, i);
            return;
        }
        
        if (obtained_number != nextParameterNumber)
        {
            std::cout << "[ERROR] Found macro definition with incorrect parameter number at position " << initial - 1 << ".\n";
            skipUntilClosingBracketOrEOF(contents, i);
            return;    
        }

        ++nParams;
        ++nextParameterNumber;

        /* Skipping all whitespaces until the next comma */
        while (j < contents.size() && std::isspace(contents[j]))
            ++j;            

        if (j > contents.size() - 1 || (contents[j] != ',' && contents[j] != ')'))
        {
            std::cout << "[ERROR] Found macro definition with incorrectly defined parameters at position " << initial - 1 << ".\n";
            std::cout << j << std::endl;
            skipUntilClosingBracketOrEOF(contents, i);
            return;  
        }

        /* All parameters processed */
        if (contents[j] == ')')
        {
            ++j;
            break;
        }

        /* Proceeding to find next parameter */
        if (contents[j] == ',')
        {
            ++j;
        }
    }

    /* Skipping all whitespaces until the next opening bracket */
    while (j < contents.size() && std::isspace(contents[j]))
        ++j;            

    if (j > contents.size() - 1 || contents[j] != '{')
    {
        std::cout << "[ERROR] Found macro definition with incorrectly defined body at position " << initial - 1 << ".\n";
        skipUntilClosingBracketOrEOF(contents, i);
        return;  
    }
    ++j;

    /* Obtaining the body of the macro */
    while (j < contents.size())
    {
        if (j == contents.size() - 1 && contents[j] != '}')
        {
            std::cout << "[ERROR] Found macro definition with incorrectly defined body at position " << initial - 1 << ".\n";
            skipUntilClosingBracketOrEOF(contents, i);
            return;
        }

        if (contents[j] != '}')
        {
            body.push_back(contents[j]);
            ++j;
        }
        else
        {
            ++j;
            break;
        }
    }

    /* Update the main index */
    i = j;

    /* Save macro definition in the macro library */
    int result = library_.add(name, body, nParams);
    if (result == -1)
    {
        std::cout << "[ERROR] Found macro definition with fewer than two parameters at position " << initial - 1 << ".\n";
    }
    else if (result == -2)
    {
        std::cout << "[ERROR] Found macro definition with a nested call or definition at position " << initial - 1 << ".\n";
    }
    else if (result == -3)
    {
        std::cout << "[ERROR] Found macro definition with incorrectly referenced parameter in the body at position " << initial - 1 << ".\n";
    }
    else if (result == -4)
    {
        std::cout << "[ERROR] Found macro definition with wrong parameter number in the body at position " << initial - 1 << ".\n";
    }
}

void MacroGenerator::processMacroCall(const std::string& contents, size_t& i, std::string& output)
{
    if (i > contents.size() - 1)
        return;

    int initial = i;
    size_t j = i;

    if ((j < contents.size() && std::isspace(contents[j])) || j >= contents.size())
    {
        std::cout << "[ERROR] Found incorrect macro definition at position " << initial - 1 << ".\n";
        skipUntilClosingBracketOrEOF(contents, i);
        return;
    }

    std::string name = "";
    std::vector<std::string> arguments;

    /* Obtaining the macro name */
    while (j < contents.size())
    {
        /* Whitespace in macro name found */
        if (std::isspace(contents[j]))
        {
            std::cout << "[ERROR] Found incorrect macro call at position " << initial - 1 << ".\n";
            skipUntilClosingParenthesisOrEOF(contents, i);
            return;
        }

        /* Opening parenthesis not found */
        if (j == contents.size() - 1 && contents[j] != '(')
        {
            std::cout << "[ERROR] Found incorrect macro call at position " << initial - 1 << ".\n";
            skipUntilClosingParenthesisOrEOF(contents, i);
            return;    
        }   

        /* Opening parenthesis (parameter block) found */
        if (contents[j] == '(')
        {
            if (name.empty())
            {
                std::cout << "[ERROR] Found macro call without a specified macro name at position " << initial - 1 << ".\n";
                skipUntilClosingParenthesisOrEOF(contents, i);
                return;
            } 
            else
            {
                ++j;
                break;
            }
        }
        name.push_back(contents[j]);
        ++j;
    }

    /* Obtaining the macro call's arguments */
    while (j < contents.size())
    {
        if (std::isspace(contents[j]))
        {
            ++j;
            continue;
        }

        if (j > contents.size() - 1)
        {
            std::cout << "[ERROR] Found macro call with incorrectly passed arguments at position " << initial - 1 << ".\n";
            skipUntilClosingParenthesisOrEOF(contents, i);
            return; 
        }

        /* Obtaining the found argument */
        std::string argument = processMacroArgument(contents, j);
        if (strcmp(argument.c_str(), "") == 0)
        {
            std::cout << "[ERROR] Found macro call with incorrectly passed arguments at position " << initial - 1 << ".\n";
            skipUntilClosingParenthesisOrEOF(contents, i);
            return;
        }
        arguments.push_back(argument);

        /* Skipping all whitespaces until the next comma */
        while (j < contents.size() && std::isspace(contents[j]))
            ++j;            

        if (j > contents.size() - 1 || (contents[j] != ',' && contents[j] != ')'))
        {
            std::cout << "[ERROR] Found macro call with incorrectly passed arguments at position " << initial - 1 << ".\n";
            skipUntilClosingParenthesisOrEOF(contents, i);
            return;  
        }

        /* All parameters processed */
        if (contents[j] == ')')
        {
            ++j;
            break;
        }

        /* Proceeding to find next parameter */
        if (contents[j] == ',')
        {
            ++j;
        }
    }

    /* Updating the main index */
    i = j;

    /* Performing a macro call */
    std::pair<int, std::string> result = library_.call(name, arguments);
    if (result.first == -1)
    {
        std::cout << "[ERROR] Found macro call to a non-existent macro at position " << initial - 1 << ".\n";
        return;
    }
    else if (result.first == -2)
    {
        std::cout << "[ERROR] Found macro call with too few arguments passed at position " << initial - 1 << ".\n";
        return;
    }
    output.append(result.second);
}

int MacroGenerator::processMacroParameterNumber(const std::string& contents, size_t& i, int nextParameterNumber)
{
    // returns -1 if the macro parameter number is incorrect
    // returns -2 if the macro parameter number contains non-digit characters
    // returns -3 if the the end of the file was reached

    /* Obtaining the parameter number */
    std::string parameterNumberStr = "";
    int parameterNumber = 0;
    while (i < contents.size())
    {
        if (contents[i] == ',' || contents[i] == ')' || std::isspace(contents[i]))
        {
            parameterNumber = std::stoi(parameterNumberStr);
            if (parameterNumber == nextParameterNumber)
                return parameterNumber;
            else
                return -1;
        }
        if (!std::isdigit(contents[i]))
        {
            return -2;
        }
        parameterNumberStr.push_back(contents[i]);
        ++i;
    }
    return -3;
}

std::string MacroGenerator::processMacroArgument(const std::string& contents, size_t& i)
{
    std::string argument = "";
    while (i < contents.size())
    {
        if (contents[i] == ',' || contents[i] == ')' || std::isspace(contents[i]))
        {
            return argument;
        }
        argument.push_back(contents[i]);
        ++i; 
    }
    return "";
}

void MacroGenerator::skipUntilClosingBracketOrEOF(const std::string& contents, size_t& i)
{
    while (i < contents.size())
    {
        if (contents[i] == '}')
        {
            ++i;
            return;
        }
        else
            ++i;
    }
    return;
}

void MacroGenerator::skipUntilClosingParenthesisOrEOF(const std::string& contents, size_t& i)
{
    while (i < contents.size())
    {
        if (contents[i] == ')')
        {
            ++i;
            return;
        }
        else
            ++i;
    }
    return;
}


void MacroGenerator::produceOutputFile(const std::string& contents)
{
    std::filesystem::path filepath = outputPath_ + "/" + outputFileName_;
    if (!std::filesystem::is_directory(outputPath_))
        std::filesystem::create_directory(outputPath_);
    std::ofstream outfile(outputPath_ + "/" + outputFileName_);
    outfile << contents;
    outfile.close();
}

void MacroGenerator::clearLogFile()
{

}

void MacroGenerator::appendToLogFile(const std::string& message)
{


}
