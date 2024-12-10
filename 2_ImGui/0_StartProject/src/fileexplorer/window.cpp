#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "rendergui/render.hpp"


void WindowClass::DrawContent()
{
    for (const auto &entry : fs::directory_iterator(currentDir))
    {
        const auto is_selected = entry.path() == select_Entry;
        const auto is_Dir = entry.is_directory();
        const auto is_File = entry.is_regular_file();
        auto entry_name = entry.path().filename().string();

        if (is_Dir)
        {
            entry_name = "[D]: " + entry_name;
        }
        else if (is_File)
        {
            entry_name = "[F]: " + entry_name;
        }

        if (ImGui::Selectable(
                entry_name.c_str(),
                is_selected)) // display just the last portion of the file path -> and just want to output a string
        {
            if (is_Dir)
            {
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
    if (ImGui::Button("Up"))
    {
        if (currentDir
                .has_parent_path()) // checking if user is at the root level
        {
            currentDir = currentDir.parent_path();
        }
    }

    /* if (ImGui::Button("Down")) {
         if (currentDir == currentDir.parent_path())
         {
             currentDir = currentDir;
         }
     }*/

    ImGui::SameLine();

    ImGui::Text(
        "Current Directory: %s",
        currentDir.string()
            .c_str()); // display current Directory -> convert to string before so all OS has the same behaivior
}

void WindowClass::DrawActions()
{
    // display current selected Dir
    if (fs::is_directory(select_Entry))
    {
        ImGui::Text("Selected Dir: %s", select_Entry.string().c_str());
    }
    // display if selected a file
    else if (fs::is_regular_file(select_Entry))
    {
        ImGui::Text("Selected file: %s", select_Entry.string().c_str());
    }
    else
    {
        ImGui::Text("Nothing Selected");
    }

    // buttons
    // specific button to rename a file
    if (ImGui::Button("Rename"))
    {
        ImGui::OpenPopup("Rename File"); // popup once clicked
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete"))
    {
        ImGui::OpenPopup("Delete File");
    }

    if (fs::is_regular_file(select_Entry) && ImGui::Button("Open"))
    { //checking if the selected is a file to be opened
        OpenFileWDefaultEditor(); // allowing the file to be opened by editor like Notepad++ for example.
    }

    renameFilePopup();
    deleteFilePopup();
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
    for (const auto &entry : fs::directory_iterator(
             currentDir)) // iterate thought the entrie directory
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
    ImGui::Text("Number of files: %u", filter_count);
}

void WindowClass::OpenFileWDefaultEditor()
{
}

void WindowClass::renameFilePopup()
{


    // Begin the pop button for renaming -> checking if need to rename -> name needs to be the same from ImGui::PopUpModal("") from within the codebase
    if (ImGui::BeginPopupModal("Rename File"))
    {
        // array for the total char for the new name
        static char buffer_name[250] = {'\0'};

        ImGui::Text("New Name");
        ImGui::InputText(
            "###newname",
            buffer_name,
            sizeof(
                buffer_name)); // giving it the size and variable to store name in

        // cancel button to leave if changed mind
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }


        if (ImGui::Button("Save"))
        {
            auto new_filename = select_Entry.parent_path() / buffer_name;

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
void WindowClass::deleteFilePopup()
{
    if (ImGui::BeginPopupModal("Delete File"))
    {
    }
}
