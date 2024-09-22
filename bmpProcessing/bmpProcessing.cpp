#include <iostream>
#include <shlobj.h>
//#include <shlwapi.h>
//#include <objbase.h>
#include "BMPGenerator.h"
#include "BMPProcessor.h"

int main(int argc, char* argv[])
{
    std::string path;
    if (argc < 2)
        path = "randomImage24.bmp";
    else
        path = argv[1];

    srand(time(0));
    system("Color 17");

    const int width = 20;
    const int height = 20;

    BMPGenerator img24(width, height);
    BMPGenerator img32(width, height);
    img24.Generate(); // by default black and white pixels are 50/50, you can specify the % of black ones with 0-100 value
    img32.Generate(); 
    img24.Export("randomImage24.bmp", 24);
    img32.Export("randomImage32.bmp", 32); // DOESN'T WORK no clue why, handcrafted image used further instead

    BMPProcessor processor;
    std::cout << path << ":\n";
    processor.OpenBMP(path);
    processor.DisplayBMP();
    processor.CloseBMP();
    std::cout << "\n32bitEXAMPLE.bmp:\n";
    processor.OpenBMP("32bitEXAMPLE.bmp");
    processor.DisplayBMP();
    processor.CloseBMP();

    std::wstring stemp = std::wstring(path.begin(), path.end());
    LPCWSTR sw = stemp.c_str();
    ShellExecute(0, 0, sw, 0, 0, SW_SHOW);
    //ShellExecute(0, 0, L"randomImage32.bmp", 0, 0, SW_SHOW);
    ShellExecute(0, 0, L"32bitEXAMPLE.bmp", 0, 0, SW_SHOW);
    std::cout << "Program done!\n";
}