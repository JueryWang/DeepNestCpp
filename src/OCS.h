#pragma once
#include "DrawEntity.h"
#include "AxisTicker.h"
#include "Camera2D.h"
#include <vector>

namespace DeepNestCpp
{
    class AABB;
    class OCS
    {
        public:
            OCS();
            ~OCS();
            void addEntity(Entity* ent);
            void deleteEntity(Entity* ent);

            void SetCanvasSizae(int width, int height);
            void ComputeScaleFitToCanvas();
            void UpdateTickers();
            void OnMouseScroll(float delta,const glm::vec2 &mousePosition);
            void OnMouseMove(const glm::vec2 &offset);
            Camera2D* GetCamera() { return camera; }

            glm::vec2 GetOCSPosWithPixelPos(const glm::vec2 &pixelPos);

        private:
            int canvasWidth;
            int canvasHeight;
            std::vector<Entity*> entityReference;
            std::vector<AxisTicker> tickers;

            AABB* objectRange = nullptr;
            AABB* canvasRange = nullptr;
            Camera2D* camera = nullptr;

            float scale = 1.0f;
            float zoomFactor = 1.149f;
            glm::vec2 mouseHoverPosition;
            float XOffset;
            float YOffset;
            float XTickUnit;
            float YTickUnit;
    };
}