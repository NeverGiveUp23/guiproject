#pragma once

#include <cstdint>
#include <string_view>
#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;
using namespace std;


class EditorClass {

    public:
    static constexpr auto bufferSize = size_t(1024); // size of the current contents of a file in char

    public:
    EditorClass() : currentFilename({})
        {
        std::memset(textBuffer, 0, 0); // reset
    }
  
    private:
        chat textBuffer[bufferSize];
        std::string currentFileName; // store the current file name


};



void render(EditorClass &windowObj) {};
