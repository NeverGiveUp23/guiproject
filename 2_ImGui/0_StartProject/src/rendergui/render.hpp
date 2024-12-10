#pragma once

#include <cstdint>
#include <string_view>
#include <filesystem>


namespace fs = std::filesystem;

class WindowClass
{
public:
    WindowClass()
        : currentDir(fs::current_path()),
          select_Entry(fs::path()){}; // starting at the path of our own exe.

    void Draw(std::string_view label);

private:
    fs::path currentDir;
    fs::path select_Entry;
    

private:
    void DrawMenu();
    void DrawFilter();
    void DrawActions();
    void DrawContent();
    void OpenFileWDefaultEditor();
    void renameFilePopup();
    void deleteFilePopup();
};

void render(WindowClass &window_obj);
