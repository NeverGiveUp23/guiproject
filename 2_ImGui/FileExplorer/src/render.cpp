#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{ // stack size 0

    constexpr static auto window_flags = ImGuiWindowFlags_NoMove; // flags for the window to not move for the user (Test)
    constexpr static auto windowSize = ImVec2(0.0F, 0.0F);
    ImGui::SetNextWindowPos(windowSize); // adjusting the window size

    // ImGui works like atack -> pushing the window on the stack.
    ImGui::Begin(label.data(), nullptr, window_flags); // stack size 1 // Putting in and testing window flags for project

    DrawMenu();
    DrawContent();
    DrawActions();
    DrawFilter(); // Filter file extenstion -> Ex. How many .txt in the dirrectory

    // stack size must be the same as the entrance of the program. Begin()
    ImGui::End(); // stack size decrement by 1  -> will result in stack error if no othe end method when calling multiple begin methods
}

void WindowClass::DrawMenu(){
    ImGui::Text("Hello Menu");
}

void WindowClass::DrawContent(){

}
void WindowClass::DrawActions(){

}

void WindowClass::DrawFilter(){

}


void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
