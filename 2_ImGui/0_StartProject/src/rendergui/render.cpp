#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{ // stack size 0


    constexpr static auto window_flags =
        ImGuiWindowFlags_NoScrollbar; // flags for the window to not move for the user (Test)

    //constexpr static auto windowPos = ImVec2(0.0F, 0.0F);
    //constexpr static auto windowSize = ImVec2(1280.0F, 720.0F);

    //ImGui::SetNextWindowPos(windowPos); // adjusting the window size
    //ImGui::SetWindowSize(windowSize);

    // ImGui works like atack -> pushing the window on the stack.
    ImGui::Begin(label.data()); // stack size 1 // Putting in and testing window flags for project
    DrawMenu();
    ImGui::Separator();
    DrawContent();
    DrawFilter();
    ImGui::Separator();
    DrawActions();


    // stack size must be the same as the entrance of the program. Begin()
    ImGui::End(); // stack size decrement by 1  -> will result in stack error if no othe end method when calling multiple begin methods
    
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("File Explorer");

}


