#include "MacroGenerator.h"

#include <filesystem>
#include <fstream>

MacroGenerator::MacroGenerator() : outputFileName_("output.txt"), outputPath_("../output")
{

}

void MacroGenerator::run(const std::string& textfile) /* TODO */
{
    std::cout << "@INPUT:\n    '" << textfile << "'\n";
    
    std::string output;
    output.reserve(512);
    
    /* Parsing the input text file */
    for ( std::size_t i = 0; i < textfile.size(); ++i )
    {   
        if ( textfile[i] == '#' ) // Add the found macro definition into the library
        {
            if ( (i + 1 <= textfile.size() && std::isspace(textfile[i + 1])) || i == textfile.size() - 1 )
            {
                std::cout << "[ERROR] Found incomplete macro definition at position " << i << ".\n";
                continue;
            }
            
            std::string name;
            std::string body;
            int nParams = 0;
            
            /* Obtaining the name of the macro */
            for (std::size_t j = i + 1; j < textfile.size(); ++j )
            {
                if ( j == textfile.size() - 1 && textfile[j] != '(' )
                {
                    std::cout << "[ERROR] Invalid macro definition at position " << i << "\n";
                    return;    
                }   

                if ( std::isspace(textfile[j]) )
                    break;
                
                if ( textfile[j] == '(' )
                    break;
                    
                name.push_back(textfile[j]);
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
        else if ( textfile[i] == '$' ) // Search of the macro in the library
        {
            if ( (i + 1 <= textfile.size() && std::isspace(textfile[i + 1])) || i == textfile.size() - 1 )
            {
                std::cout << "[ERROR] Found incomplete macro call at position " << i << ".\n";
                continue;
            }
            
            
            
            
            
        }
        else // Copy free text to the output
        {
            output.push_back(textfile[i]);    
        }
    }
    
    std::cout << "@OUTPUT:\n    '" << output << "'\n";    
    produceOutputFile(output);
}

int MacroGenerator::processMacroDefinition(const std::string& textfile, int& index)
{
    return 0;
}

int MacroGenerator::processMacroCall(const std::string& textfile, int& index)
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