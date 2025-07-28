#pragma once
#include <QPoint>
#include <Windows.h>
#include <QWindow>
#include <QWidget>
#include <vector>
#include "DrawEntity.h"
#include "Shader.h"
#include "OpenGLContext.h"
#include "OpenGLRenderWindow.h"

namespace DeepNestCpp
{
    class Entity;
    class OCS;

    class Canvas : public OpenGLRenderWindow
    {
        friend class Sketch;
        public:
            Canvas(int width,int height,bool isMainCanvas);
            ~Canvas();
            
            void AddEntity(Entity* ent);
            void UpdateOCS();
            void SetRenderMode(RenderMode mode) { this->mode = mode; }

        protected:
            virtual bool eventFilter(QObject* obj, QEvent* event) override;
            virtual void updateGL(Sketch* sketch) override;

        private: 
            void DrawTickers();

        private: 
            RenderMode mode = RenderMode::Normal;

            OCS* ocsSys = nullptr;
            QPoint lastMousePos;
            std::vector<Polyline2D*> envolopCurve;
            std::vector<Entity*> auxiliary;
            std::vector<Entity*> entities;
            std::vector<EntCompound*> parts;
            bool firstResize;
            bool isDragging;
            bool isMainCanvas;
    };
}