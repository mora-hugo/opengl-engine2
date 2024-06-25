#include <iostream>
#include "App.h"
#include "File/FileIO.h"

int main()
{
    //HC::App app;

    //app.Run();
    HC::FileIO file("../ez.txt");
    file.Write(reinterpret_cast<const uint8_t*>(10000), 11);
    file.CloseFile();
    return 0;
}


