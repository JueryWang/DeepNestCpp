#pragma once
#include "DrawEntity.h"
#include "AxisTicker.h"
#include "Camera2D.h"
#include <vector>

namespace DeepNestCpp
{
    class AABB;
    class Sketch;

    class OCS
    {
        public:
            OCS(std::shared_ptr<Sketch> SK);
            ~OCS();

            void SetCanvasSizae(int width, int height);
            void ComputeScaleFitToCanvas();
            void UpdateTickers();
            void OnMouseScroll(float delta,const glm::vec2 &mousePosition);
            void OnMouseMove(const glm::vec2 &offset);
            Camera2D* GetCamera() { return camera; }

            glm::vec2 GetOCSPosWithPixelPos(const glm::vec2 &pixelPos);

        public:
            int canvasWidth;
            int canvasHeight;
            std::vector<AxisTicker> tickers;

            std::shared_ptr<Sketch> sketch;
            AABB* objectRange = nullptr;
            AABB* canvasRange = nullptr;
            Camera2D* camera = nullptr;

            float scale = 1.0f;
            float zoomFactor = 1.149f;
            float XOffset;
            float YOffset;
            float XTickUnit;
            float YTickUnit;
            bool genTickers = false;
    };
}