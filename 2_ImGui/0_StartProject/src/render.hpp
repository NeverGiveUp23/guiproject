#pragma once

#include <cstdint>
#include <string_view>

class WindowClass
{
public:
    void Draw(std::string_view label);

private:
    void DrawMenu();
    void DrawFilter();
    void DrawActions();
    void DrawContent();
};

void render(WindowClass &window_obj);
