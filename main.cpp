#include "src/Application.h"

#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        Application app;
        return app.run(argc, argv);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error." << std::endl;
        return 1;
    }
}