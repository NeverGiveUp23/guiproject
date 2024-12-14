#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "plotting.hpp"

using namespace std;

void PlottingClass::DrawPlot(string_view label)
{
    ImGui::Begin(label.data());
    DrawSelection();
    DrawPlot();
    ImGui::End();
}


void renderPlot(PlottingClass &plotting_obj)
{
    plotting_obj.DrawPlot("Plotting");
}
