#include "MacroGenerator.h"

#include <filesystem>
#include <fstream>

MacroGenerator::MacroGenerator() : outputFileName_("output.txt"), outputPath_("../output")
{

}

std::string MacroGenerator::process(const std::string& contents)
{
    std::string output;
    output.reserve(512);
    
    /* Parsing the input text file */
    for ( std::size_t i = 0; i < contents.size(); ++i )
    {   
        if ( contents[i] == '#' ) // Add the found macro definition into the library
        {
            if ( (i + 1 <= contents.size() && std::isspace(contents[i + 1])) || i == contents.size() - 1 )
            {
                std::cout << "[ERROR] Found incomplete macro definition at position " << i << ".\n";
                continue;
            }
            
            std::string name;
            std::string body;
            int nParams = 0;
            
            /* Obtaining the name of the macro */
            for (std::size_t j = i + 1; j < contents.size(); ++j )
            {
                if ( j == contents.size() - 1 && contents[j] != '(' )
                {
                    std::cout << "[ERROR] Invalid macro definition at position " << i << "\n";
                    return "";    
                }   

                if ( std::isspace(contents[j]) )
                    break;
                
                if ( contents[j] == '(' )
                    break;
                    
                name.push_back(contents[j]);
            }
            
            if ( name.empty() )
            {
                std::cout << "[ERROR] Found macro definition without a specified macro name.\n";
                ++i;
            }
            else
            {
                i += name.size();
            }
            
            
            
            
            
        }
        else if ( contents[i] == '$' ) // Search of the macro in the library
        {
            if ( (i + 1 <= contents.size() && std::isspace(contents[i + 1])) || i == contents.size() - 1 )
            {
                std::cout << "[ERROR] Found incomplete macro call at position " << i << ".\n";
                continue;
            }
            
            
            
            
            
        }
        else // Copy free text to the output
        {
            output.push_back(contents[i]);    
        }
    }
    produceOutputFile(output);
    return output;
}

int MacroGenerator::processMacroDefinition(const std::string& contents, int& index)
{
    return 0;
}

int MacroGenerator::processMacroCall(const std::string& contents, int& index)
{
    return 0;
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