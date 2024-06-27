#include <iostream>
#include "App.h"
#include "File/FileIO.h"
#include "Ressources/ResourceManager.h"
int main()
{
    //HC::App app;

    //app.Run();
    /*
    HC::FileIO file("../ez.txt");
    std::array<uint8_t, 2> v {10,24};
    file.Write(reinterpret_cast<const uint8_t*>(v.data()), v.size()*sizeof(uint8_t));
    file.CloseFile();
     */

    auto manager = HC::ResourceManager::GetInstance();
    auto ez = manager->Load<HC::FileResource>("../yolerap.txt");
    ez->bytes = {'c','o','u','c','o','z'};
    ez->Save();
    return 0;
}


