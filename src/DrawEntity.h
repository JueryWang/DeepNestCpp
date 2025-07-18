#pragma once
#include "../lib/OGL/glad/glad.h"
#include "AABB.h"
#include "../lib/OGL/glm/glm.hpp"
#include <vector>

namespace DeepNestCpp
{
    enum class EntityType
    {
        Point,
        Line,
        Circle,
        Ellipse,
        Polyline,
        Arc,
        Spline,
        Rectangle
    };

    class Entity
    {
        public:

            virtual EntityType GetType() const = 0;
            virtual void Paint() = 0;
            virtual void Move(glm::vec3 offset) = 0;
            virtual void Rotate(glm::vec3 center,float angle) = 0;
            virtual void Mirror(glm::vec3 center) = 0;
            virtual void SetParameter(int paramCount, ...) = 0;
            virtual void ToNcInstruction() = 0;

            virtual ~Entity()
            {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                if (vao != 0)
                {
                    glDeleteVertexArrays(1, &vao);
                    vao = 0;
                }
                if (vbo != 0)
                {
                    glDeleteBuffers(1, &vbo);
                    vbo = 0;
                }

                delete aabb;
            }
            
        protected:
            static long counter;
            GLuint vao;
            GLuint vbo;
        public:
            AABB* aabb;
            std::vector<float> jointPoints;
            float jointLength;

            glm::vec3 startProcessLocation;
            glm::mat4 transformMatrix;
            bool isSelected;
    };

    class Point2D : public Entity{
        public:
            Point2D(glm::vec3 point);
            ~Point2D();
            virtual EntityType GetType() const override { return EntityType::Point; }
            virtual void Paint() override;
            virtual void Move(glm::vec3 offset) override;
            virtual void Rotate(glm::vec3 center, float angle) override;
            virtual void Mirror(glm::vec3 center) override;
            virtual void SetParameter(int paramCount, ...) override;
            virtual void ToNcInstruction() override;

        private:
            glm::vec3 point;
    };

    class Line2D : public Entity{
        public:
            Line2D(glm::vec3 start, glm::vec3 end);
            ~Line2D();
            virtual EntityType GetType() const override { return EntityType::Line; }
            virtual void Paint() override;
            virtual void Move(glm::vec3 offset) override;
            virtual void Rotate(glm::vec3 center, float angle) override;
            virtual void Mirror(glm::vec3 center) override;
            virtual void SetParameter(int paramCount, ...) override;
            virtual void ToNcInstruction() override;

        private:
            glm::vec3 start;
            glm::vec3 end;
    };

    class Arc2D : public Entity{
        public:
            Arc2D(glm::vec3 center, float radius, float startAngle, float endAngle);
            ~Arc2D();
            virtual EntityType GetType() const override { return EntityType::Arc; }
            virtual void Paint() override;
            virtual void Move(glm::vec3 offset) override;
            virtual void Rotate(glm::vec3 center, float angle) override;
            virtual void Mirror(glm::vec3 center) override;
            virtual void SetParameter(int paramCount, ...) override;
            virtual void ToNcInstruction() override;

            void GenerateArcSamples(float startAngle,float endAngle,const glm::vec3& center,std::vector<glm::vec3>& samples);

        private:
            glm::vec3 start;
            glm::vec3 end;
            glm::vec3 center;
            float radius;
            float startAngle;
            float endAngle;
            std::vector<glm::vec3> arcSamples;
    };

    class Circle2D : public Entity{
        public:
            Circle2D(glm::vec3 center, float radius);
            ~Circle2D();
            virtual EntityType GetType() const override { return EntityType::Circle; }
            virtual void Paint() override;
            virtual void Move(glm::vec3 offset) override;
            virtual void Rotate(glm::vec3 center, float angle) override;
            virtual void Mirror(glm::vec3 center) override;
            virtual void SetParameter(int paramCount, ...) override;
            virtual void ToNcInstruction() override;

            void GenerateCircleSamplePoints(const glm::vec3& center,float radius,int stepAngle,std::vector<glm::vec3> &samples);

        private:
            std::vector<glm::vec3> circleSamples;
            glm::vec3 center;
            float radius;
    };

    class Ellipse2D : public Entity{
        public:
            Ellipse2D(glm::vec3 center, float radiusX, float radiusY);
            ~Ellipse2D();
            virtual EntityType GetType() const override { return EntityType::Ellipse; }
            virtual void Paint() override;
            virtual void Move(glm::vec3 offset) override;
            virtual void Rotate(glm::vec3 center, float angle) override;
            virtual void Mirror(glm::vec3 center) override;
            virtual void SetParameter(int paramCount, ...) override;
            virtual void ToNcInstruction() override;

            void GenerateEllipseSamplePoints(glm::vec3 center,float radiusX,float radiusY,int stepAngle,std::vector<glm::vec3>& samples);
        private:
            std::vector<glm::vec3> ellipseSamples;
            glm::vec3 center;
            float radiusX;
            float radiusY;
    };

    class Polyline2D : public Entity{
        public:
            Polyline2D(const std::vector<glm::vec3>& points,bool isClosed);
            ~Polyline2D();
            virtual EntityType GetType() const override { return EntityType::Polyline; }
            virtual void Paint() override;
            virtual void Move(glm::vec3 offset) override;
            virtual void Rotate(glm::vec3 center, float angle) override;
            virtual void Mirror(glm::vec3 center) override;
            virtual void SetParameter(int paramCount, ...) override;
            virtual void ToNcInstruction() override;

        private:
            std::vector<glm::vec3> nodes;
            std::vector<float> bulges;
            bool isClosed;
    };

    class Spline2D : public Entity{
        public:
            Spline2D(const std::vector<glm::vec3>& controlPoints, const std::vector<float> knots);
            ~Spline2D();
            virtual EntityType GetType() const override { return EntityType::Spline; }
            virtual void Paint() override;
            virtual void Move(glm::vec3 offset) override;
            virtual void Rotate(glm::vec3 center, float angle) override;
            virtual void Mirror(glm::vec3 center) override;
            virtual void SetParameter(int paramCount, ...) override;
            virtual void ToNcInstruction() override;

            void GenerateSplineSamplePoints(const std::vector<glm::vec3>& controlPoints,std::vector<glm::vec3> &samples);
        private:
            GLuint vao_cntl;
            GLuint vbo_cntl;
            std::vector<glm::vec3> controlPoints;
            std::vector<glm::vec3> splineSamples;
            std::vector<float> knots;
    };

    class EntCompound
    {
        public:
            EntCompound() = default;
            ~EntCompound() = default;

            void AddEntity(Entity* entity)
            {
                entities.push_back(entity);
            }

        public:
            std::vector<Entity*> entities;
            AABB aabb;
            glm::vec3 baseTransform;
    };
}