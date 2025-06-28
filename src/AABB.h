#pragma once
#include "../lib/OGL/glm/glm.hpp"
#include <algorithm>

namespace DeepNestCpp
{
    class AABB
    {
    public:
        AABB(const glm::vec3& min, const glm::vec3& max)
        {
            this->min.x = std::min(min.x, max.x);
            this->min.y = std::min(min.y, max.y);
            this->min.z = std::min(min.z, max.z);
            this->max.x = std::max(min.x, max.x);
            this->max.y = std::max(min.y, max.y);
            this->max.z = std::max(min.z, max.z);
        }

        AABB(const AABB& other)
            : min(other.min), max(other.max) {}

        AABB(AABB* other) : min(other->min), max(other->max) {}


        void Union(const glm::vec3& p)
        {
            min.x = std::min(min.x, p.x);
            min.y = std::min(min.y, p.y);
            min.z = std::min(min.z, p.z);
            max.x = std::max(max.x, p.x);
            max.y = std::max(max.y, p.y);
            max.z = std::max(max.z, p.z);
        }

        void Union(AABB* other)
        {
            min.x = std::min(min.x, other->min.x);
            min.y = std::min(min.y, other->min.y);
            min.z = std::min(min.z, other->min.z);
            max.x = std::max(max.x, other->max.x);
            max.y = std::max(max.y, other->max.y);
            max.z = std::max(max.z, other->max.z);
        }

        float XRange() const { return max.x - min.x; }
        float YRange() const { return max.y - min.y; }
        float ZRange() const { return max.z - min.z; }

        const glm::vec3& getMin() const { return min; }
        const glm::vec3& getMax() const { return max; }

        glm::vec3 Center()
        {
            return glm::vec3(
                (min.x + max.x) * 0.5f,
                (min.y + max.y) * 0.5f,
                (min.z + max.z) * 0.5f
            );
        }

        void Translate(const glm::vec3& translation)
        {
            min += translation;
            max += translation;
        }

        void Multiply(float scale)
        {
            min.x *= scale;
            min.y *= scale;
            min.z *= scale;
        }

    public:
        glm::vec3 min;
        glm::vec3 max;
    };
}
