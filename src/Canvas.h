#pragma once
#include <QPoint>
#include <Windows.h>
#include <QWindow>
#include <QWidget>
#include <vector>
#include "../lib/OGL/glm/glm.hpp"
#include "../lib/OGL/GLFW/glfw3.h"=
#include "Shader.h"

namespace DeepNestCpp
{
    class Entity;
    class OCS;

    class Canvas : QWindow
    {
        public:
            Canvas(int width,int height,bool isMainCanvas);
            ~Canvas();
            
            void addEntity(Entity* ent);
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
            std::vector<Entity*> entities;
            std::vector<EntCompound*> parts;
            bool firstResize;
            bool isDragging;
            bool isMainCanvas;
    };
}