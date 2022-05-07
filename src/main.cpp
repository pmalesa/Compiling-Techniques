#include <iostream>
#include <fstream>

#include "MacroGenerator.h"

int main(int argc, char* argv[])
{
    if ( argc != 2 )
    {
        std::cout << "[ERROR] Inappropriate number of input arguments.\n"; 
        return -1;
    }

    std::string filename = argv[1];
    std::string contents;
    std::string line;
    std::ifstream file(filename);
    if ( !file.is_open() )
    {
        std::cout << "[ERROR] Unable to open '" << filename << "'.\n";
        return -2;
    }

    while ( std::getline(file, line) )
        contents.append(line);

    file.close();

    std::cout << "@RUNNING MACROGENERATOR\n";
    MacroGenerator mg;
    mg.run(contents);
    
    std::string name = "MACRO";
    std::string body = "body_1 &1 body_2 &2";
    int nParams = 3;
    
    MacroLibrary ml;
    int status = ml.add(name, body, nParams);
    std::cout << "Status: " << status << "\n";
    
    std::vector<std::string> params;
    params.push_back("FIRST");
    params.push_back("SECOND");
    params.push_back("THIRD");
    params.push_back("FOURTH");

    std::pair<int, std::string> result = ml.call("MACRO", params);
    std::cout << "Result: " << result.second << "\nStatus: " << result.first << "\n";
    
    return 0;
}
