/*
This is the window functions for the file explorer section of the gui.
*/

#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <string>

#include "rendergui/render.hpp"


void WindowClass::DrawContent()
{
    for (const auto &entry : fs::directory_iterator(currentDir))
    {
        const auto is_selected = entry.path() == select_Entry;
        const auto is_Dir = entry.is_directory();
        const auto is_File = entry.is_regular_file();
        auto entry_name = entry.path().filename().string();

        if (is_Dir) {
            entry_name = "[D]: " + entry_name;
        }else if (is_File) {
            entry_name = "[F]: " + entry_name;
        }

        if (ImGui::Selectable(
                entry_name.c_str(),
                is_selected)) { // display just the last portion of the file path -> and just want to output a string
            if (is_Dir){
                currentDir /= entry.path().filename();
            }
            else
            {
                select_Entry = entry.path();
            }
        }
    }
}


void WindowClass::DrawMenu()
{
    ImGui::Text("Hello Menu");
    // when button is clicked -> adjusting button size
    if (ImGui::Button("Back")){
        if (currentDir.has_parent_path()) // checking if user is at the root level
        {
            currentDir = currentDir.parent_path();
        }
    }

    ImGui::SameLine();

    ImGui::Text(
        "Current Directory: %s",
        currentDir.parent_path()
            .string()
            .c_str()); // display current Directory -> convert to string before so all OS has the same behaivior

    
}


void WindowClass::displayPerm(fs::perms p)
{
    auto show = [=](char op, fs::perms perm) {
        auto noPerm = (fs::perms::none == (perm & p) ? '-' : op);
        ImGui::SameLine();
        ImGui::Text("%c", noPerm);
    };

    show('r', fs::perms::owner_read);
    show('w', fs::perms::owner_write);
    show('x', fs::perms::owner_exec);
    show('r', fs::perms::group_read);
    show('w', fs::perms::group_write);
    show('x', fs::perms::group_exec);
    show('r', fs::perms::others_read);
    show('w', fs::perms::others_write);
    show('x', fs::perms::others_exec);
}

void WindowClass::DrawActions()
{

    // display current selected Dir
    if (fs::is_directory(select_Entry)) {
        ImGui::Text("Selected Dir: %s", select_Entry.string().c_str());
    }
    // display if selected a file
    else if (fs::is_regular_file(select_Entry)) {
        ImGui::Text("Selected file: %s", select_Entry.filename().string().c_str());
        ImGui::Text("Permissions: %i", fs::status(select_Entry).permissions());
    }
    else {
        ImGui::Text("Nothing Selected");
    }

    // buttons
    // specific button to rename a file
    if (ImGui::Button("Rename")) {
        renameDialogOpen = true;
        ImGui::OpenPopup("Rename File"); // popup once clicked
    }
    renameFilePopup();

    ImGui::SameLine();

    if (ImGui::Button("Delete")) {
        deleteDialogOpen = true;
        ImGui::OpenPopup("Delete File");
    }
    deleteFilePopup();

    // checking to see if entry is a file for open button and function
    if (fs::is_regular_file(select_Entry) && ImGui::Button("Open")) { //checking if the selected is a file to be opened
        OpenFileWDefaultEditor(); // allowing the file to be opened by editor like Notepad++ for example.
    }

    
}

void WindowClass::DrawFilter()
{
    static char ext_filter[16] = {"\0"};

    ImGui::Text("Filter ");
    ImGui::SameLine();
    ImGui::InputText(
        "###Input Filter",
        ext_filter,
        sizeof(
            ext_filter)); // ### -> to not display that text. ext_filter an array of chars for the text and the size being no more than the sizeof ext_filter

    // chewck if user entered anything
    if (std::strlen(ext_filter) == 0) // checking input length from user
    {
        return;
    }
    // message when too many chars
    if (std::strlen(ext_filter) == 15) // also checking if the length is the max
    {
        ImGui::TextColored(
            ImVec4(1.0F,
                   2.4F,
                   0.0F,
                   2.4F), // -> display message in yellow colored text
            "Too many characters");
        return;
    }

    //  count variable for # of files in dir
    auto filter_count = std::size_t{0};
    for (const auto &entry : fs::directory_iterator(currentDir)) // iterate thought the entrie directory
    {
        // if not a file, just ignore and continue
        if (!fs::is_regular_file(entry))
        {
            continue;
        }
        // up the count when # of files are in the current Dir
        if (entry.path().extension().string() == ext_filter)
        {
            ++filter_count;
        }
    }
    // display count
    ImGui::Text("Number of %s files: %u", ext_filter, filter_count);
}


// open file from gui
void WindowClass::OpenFileWDefaultEditor(){
    #ifdef _WIN32
        const auto command = "start \"\" \"" + select_Entry.string() + "\"";
    #elif __APPLE__
        const auto command = "open \"" + select_Entry.string() + "\"";
    #else
        const auto command = "xdg-open \"" + select_Entry.string() + "\"";
    #endif

    std::system(command.c_str());
}

void WindowClass::renameFilePopup()
{
    static bool dialog_open = false;

    // Begin the pop button for renaming -> checking if need to rename -> name needs to be the same from ImGui::PopUpModal("") from within the codebase
    if (ImGui::BeginPopupModal("Rename File", &renameDialogOpen))
    {
        // array for the total char for the new name
        static char buffer_name[250] = {'\0'};

        ImGui::Text("New Name");
        ImGui::InputText(
            "###newname",
            buffer_name,
            sizeof(
                buffer_name)); // giving it the size and variable to store name in


        if (ImGui::Button("Rename"))
        {
            auto new_filename = select_Entry.parent_path() / buffer_name;
            if (renameFile(select_Entry, new_filename))
            {
                renameDialogOpen = false;
                select_Entry = new_filename;
                std::memset(buffer_name, 0, sizeof(0)); // return input box back to 0 -> as in empty string ""
            }
        }
        ImGui::SameLine();

        // cancel button to leave if changed mind
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }



        ImGui::EndPopup();
    }
}
void WindowClass::deleteFilePopup()
{
    // When the delete button is clicked
    if (ImGui::BeginPopupModal("Delete File", &deleteDialogOpen)){
        // message popup to verify delete
        ImGui::Text("Are you sure you want to delete - %s?",
                    select_Entry.filename().string().c_str());

        if (ImGui::Button("Yes"))
        {
            // match selected entry to delete
            if (deleteFile(select_Entry))
            {
                // clear the delete file/delete
                select_Entry.clear();
            }
            // close popup
            deleteDialogOpen = false;
        }

        ImGui::SameLine();

        // if no, just close popup
        if (ImGui::Button("No"))
        {
            deleteDialogOpen = false;
        }
        ImGui::EndPopup();
    }
}


bool WindowClass::renameFile(const fs::path &old_path, const fs::path &new_path)
{
    try {
        fs::rename(old_path, new_path);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what();
        return false;
    }

}

bool WindowClass::deleteFile(const fs::path& path) {
    try
    {
        fs::remove(path);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what();
        return false;
    }
}
