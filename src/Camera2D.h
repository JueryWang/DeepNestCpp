#pragma once
#include "../lib/OGL/glm/glm.hpp"
#include "../lib/OGL/glm/gtc/matrix_transform.hpp"
#include "../lib/OGL/glm/gtc/type_ptr.hpp"
#include "AABB.h"

namespace DeepNestCpp
{
    class Camera2D
    {
        public:
            Camera2D(AABB* bbox) : range(bbox)
            {

            }

            void UpdateRange(AABB* range)
            {
                *this->range = *range;
            }

            glm::mat4 GetOrthoGraphicMatrix()
            {
                AABB rangeNew(glm::vec3(range->min.x,range->min.y,0.0f),glm::vec3(range->max.x,range->max.y,0.0f));
                glm::vec3 offset = range->Center() - rangeNew.Center();
                rangeNew.Translate(offset);

                return glm::ortho(rangeNew.min.x,rangeNew.max.x,rangeNew.min.y,rangeNew.max.y);
            }

            glm::mat4 GetViewMatrix()
            {
                return glm::mat4(1.0f);
            }
        private:
            AABB* range;
    };    
} // namespace DeepNestCpp
