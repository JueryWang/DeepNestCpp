#pragma once
#include "../lib/OGL/glad/glad.h"
#include "../lib/OGL/glm/glm.hpp"
#include <vector>
#include <iostream>
#include <vector>

namespace DeepNestCpp
{
    enum TickType
    {
        Main,
        Sub
    };

    enum AxisType
    {
        X,
        Y
    };

    class AxisTicker
    {
        float value;
        glm::vec3 tickCoord[2];
        TickType Ttype;
        AxisType Atype;

    public:
        AxisTicker(float _value,const std::vector<glm::vec3>& oglPosition, TickType _Ttype, AxisType _AType)
        {
            tickCoord[0] = oglPosition[0];
            tickCoord[1] = oglPosition[1];
            value = _value;
            Ttype = _Ttype;
            Atype = _AType;
        }
    };
}