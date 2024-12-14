#include <iostream>
#include <array>
#include <set>
#include <cmath>
#include <format>
#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <string_view>
#include "plotting.hpp"

using namespace std;


void PlottingClass::DrawPlot(){

    static constexpr auto num_points = 10'000;
    static constexpr auto x_min = -100.0;
    static constexpr auto x_max = 100.0;

    static const auto x_step = (abs(x_max) + abs(x_min)) / num_points;

    static auto x_val = array<double, num_points>{};
    static auto y_val = array<double, num_points>{};

    // checking if plot is not entered -> if the function size is 0 or is none -> de-referencing of *selectedFunctions to get the core value if NONE check
    if (selectedFunctions.size() == 0 || (selectedFunctions.size() == 1 && *selectedFunctions.begin() == Function::NONE)) {
        ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);
        ImPlot::EndPlot();
        return;
    }

    ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);
    for (const auto &function : selectedFunctions) {
        auto x = x_min;
        for (size_t i = 0; i < num_points; ++i){
            x_val[i] = x;
            y_val[i] = evalFunction(function, x);
            x += x_step;
        }

        const auto plot_label = fmt::format("##function{}", static_cast<int>(function));
        ImPlot::PlotLine(plot_label.data(),
                         x_val.data(),
                         y_val.data(),
                         num_points);
    }

    ImPlot::EndPlot();
}

void PlottingClass::DrawSelection(){

    for (const auto func_name : function_names) {
        const auto current_func = functionNameMap(func_name);
        auto selected = selectedFunctions.contains(current_func);

        if (ImGui::Checkbox(func_name.data(), &selected)) {
            if (selected) {
                selectedFunctions.insert(current_func);
            }
            else{
                selectedFunctions.erase(current_func);
            }
        }
    }
}

PlottingClass::Function PlottingClass::functionNameMap(string_view function_name){

    if (string_view{"sin(x)"} == function_name)
        return PlottingClass::Function::SIN;
    if (string_view{"cos(x)"} == function_name)
        return PlottingClass::Function::COS;

    return PlottingClass::Function::NONE;
}

double PlottingClass::evalFunction(const Function function, const double x){

    switch (function) {
    case Function::COS:
        return cos(x);
        break;
    case Function::SIN:
        return sin(x);
        break;
    case Function::NONE:
    default:
        return 0.0;
    }
}
