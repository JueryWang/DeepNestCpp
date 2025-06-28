#pragma once
#include <string>
#include "../lib/OGL/glm/glm.hpp"

namespace DeepNestCpp
{
    enum LineType
    {
        Continious,
        DashedLine,
        DashedLineWithInterp,
    };

    struct Layer
    {
        std::string name;
        float lineWeight;
        glm::vec4 color;
        LineType lineType;
    };
}
