#include <iostream>
#include <fstream>

#include "MacroGenerator.h"
#include "TestModule.h"

/* Comment this line to disable tests */
// #define RUN_TESTS

int main(int argc, char* argv[])
{
    /* -------------------------------------TESTS-------------------------------------- */
#ifdef RUN_TESTS
    int testRetVal = TestModule::runTests();
    // if (testRetVal != 0)
    //     return testRetVal;
#endif

    /* ------------------------------------PROGRAM------------------------------------- */
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
    char c;
    while ( file.get(c) )
        contents += c;
    file.close();

    std::cout << "@RUNNING MACROGENERATOR\n";
    std::cout << "@INPUT:\n";
    std::cout << "*---------------------------------------------*\n";
    std::cout << contents << "\n";
    std::cout << "*---------------------------------------------*\n\n";

    MacroGenerator mg;
    std::string output = mg.process(contents);

    std::cout << "\n@OUTPUT:\n";
    std::cout << "*---------------------------------------------*\n";
    std::cout << output << "\n";
    std::cout << "*---------------------------------------------*\n";
    
    return 0;
}
