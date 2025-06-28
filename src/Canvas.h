#pragma once
#include <QPoint>
#include <Windows.h>
#include <QWindow>
#include <QWidget>
#include <vector>
#include "../lib/OGL/glm/glm.hpp"
#include "../lib/OGL/GLFW/glfw3.h"
#include "DrawEntity.h"
#include "OCS.h"
#include "Shader.h"

namespace DeepNestCpp
{
    class Canvas : QWindow
    {
        public:
            Canvas(int width,int height,bool isMainCanvas);
            ~Canvas();
            
            void Paint();
            void UpdateOCS();

        protected:
            bool eventFilter(QObject* obj, QEvent* event) override;

        private: 
            void DrawTickers();

        private: 
            GLFWwindow* window;
            static Shader drawEntityShader;
            static Shader drawTickerShader;
            static Shader drawTickerTextShader;
            OCS* ocsSys = nullptr;
            QPoint lastMousePos;
            EntCompound* part;
            bool firstResize;
            bool isDragging;
            bool isMainCanvas;
    };
}