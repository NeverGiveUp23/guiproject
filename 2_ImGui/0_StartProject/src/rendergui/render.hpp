#pragma once

#include <cstdint>
#include <string_view>
#include <filesystem>


namespace fs = std::filesystem;
using namespace std;

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
    fs::path rootPath;

    bool renameDialogOpen = false;
    bool deleteDialogOpen = false;
    

private:
    void DrawMenu();
    void DrawFilter();
    void DrawActions();
    void DrawContent();
    void OpenFileWDefaultEditor();
    void renameFilePopup();
    void deleteFilePopup();
    bool renameFile(const fs::path &old_path, const fs::path &new_path);
    bool deleteFile(const fs::path &path);
    void displayPerm(fs::perms p);


};

void render(WindowClass &window_obj);
