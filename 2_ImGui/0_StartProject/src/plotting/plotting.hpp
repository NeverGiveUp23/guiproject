#pragma once

#include <cstdint>
#include <string_view>
#include <iostream>
#include <array>
#include <set>

using namespace std;

class PlottingClass
{
public:
    constexpr static auto function_names =
        array<string_view, 3>{"unkown", "sin(x)", "cos(x)"};

    enum class Function {
        NONE,
        SIN,
        COS
    };

public:
    PlottingClass() : selectedFunctions({}) {};
    void DrawPlot(string_view label);

private:
    void DrawSelection();
    void DrawPlot();

    Function functionNameMap(string_view function_name);
    double evalFunction(const Function function, const double x);


public:
    set<Function> selectedFunctions; // selecting both sin cos at the same time and store them

};

void renderPlot(PlottingClass &plotting_obj);
