// HEADER
#include "pch.hpp"

// WINDOWS
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)

// Include Windows API
#include <Windows.h>

// Include stringbuf
#include <sstream>

#endif
// WINDOWS

// ===========================================================
// MAIN
// ===========================================================

void loadLib()
{
    std::cout << "Library loaded." << std::endl;
    const int sumResult = Sum(2, 2);
    std::cout << "result = " << std::to_string(sumResult) << std::endl;
}

int main()
{

    // Print
    std::cout << "this  is a test" << std::endl;

    // Use SHARED Library.
    loadLib();

    // Wait input.
    std::cin.get();

    // OK
    return 0;
}

// -----------------------------------------------------------
