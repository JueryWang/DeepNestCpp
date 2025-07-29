#define GLFW_EXPOSE_NATIVE_WIN32
#include "Canvas.h"
#include "OCS.h"
#include "Sketch.h"
#include "GLWidget.h"
#include "DrawEntity.h"
#include <Windows.h>
#include <QMouseEvent>
#include "../lib/OGL/GLFW/glfw3.h"

namespace DeepNestCpp
{

    Canvas::Canvas(std::shared_ptr<Sketch> sketch,int width,int height, bool isMainCanvas) : OpenGLRenderWindow(width,height,""), isMainCanvas(isMainCanvas)
    {
        firstResize = true;
        ocsSys = new OCS(sketch);
        m_sketch = sketch;
        ocsSys->genTickers = isMainCanvas;
        ocsSys->canvasWidth = width;
        ocsSys->canvasHeight = height;
        UpdateOCS();

        this->Resize(QSize(width, height));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Shader* Normal_Shader = new Shader("drawEntity.vert", "drawEntity.frag");
        Shader* ShowArrow_Shader = new Shader("drawEntity.vert","drawEntity.frag", "addArrow.geom");
        Shader* drawTicker_Shader = new Shader("drawTickText.vert", "drawTickText.frag");
        m_shaderMap[RenderMode::Normal] = Normal_Shader;
        m_shaderMap[RenderMode::ShowArrow] = ShowArrow_Shader;
        m_shaderMap[RenderMode::DrawTickers] = drawTicker_Shader;
    }

    Canvas::~Canvas()
    {
        m_sketch.reset();
    }

    void Canvas::AddEntity(Entity* ent)
    {
        entities.push_back(ent);
    }

    void Canvas::UpdateOCS()
    {
        ocsSys->ComputeScaleFitToCanvas();
        if (isMainCanvas)
        {
            ocsSys->UpdateTickers();
        }
    }

    bool Canvas::eventFilter(QObject* obj, QEvent* event)
    {
        GLWidget* glwgt = (GLWidget*)this->parent();
        
        switch (event->type())
        {
            case QEvent::Resize:
            {
                int width, height;
                glfwGetWindowSize(m_window.get(), &width, &height);
                if (ocsSys != nullptr)
                {
                    ocsSys->SetCanvasSizae(width, height);
                    if (firstResize)
                    {
                        ocsSys->ComputeScaleFitToCanvas();
                        ocsSys->UpdateTickers();
                        firstResize = false;
                    }
                }
                break;
            }
            case QEvent::MouseButtonPress:
            {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                if (mouseEvent->buttons() & Qt::MiddleButton)
                {
                    isDragging = true;
                    lastMousePos = mouseEvent->pos();
                }
                break;
            }
            case QEvent::MouseButtonRelease:
            {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                if (mouseEvent->button() & Qt::MiddleButton)
                {
                    isDragging = false;
                }
                break;
            }
            case QEvent::MouseMove:
            {
                if (isDragging)
                {
                    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                    QPoint currentMousePos = mouseEvent->pos();
                    QPoint offset = currentMousePos - lastMousePos;

                    lastMousePos = currentMousePos;
                    ocsSys->OnMouseMove(glm::vec2(offset.x()*0.1f, offset.y()*0.1f));
                }
                break;
            }
            case QEvent::Wheel:
            {
                if (!isDragging)
                {
                    QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
                    float delta = wheelEvent->angleDelta().y() * 0.1;
                    lastMousePos = wheelEvent->position().toPoint();

                    ocsSys->OnMouseScroll(delta, glm::vec2(lastMousePos.x(),lastMousePos.y()));
                }
                break;
            }
        }

        return glwgt->eventFilter(obj,event);
    }

    void Canvas::updateGL()
    {
        GLFWwindow* windowInst = m_window.get();
        if (windowInst)
        {
            glfwMakeContextCurrent(windowInst);
            glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Shader* shaderUsed = m_shaderMap[mode];
            shaderUsed->use();
            int width, height;
            glfwGetWindowSize(windowInst, &width, &height);

            glm::mat4 ortho = ocsSys->camera->GetOrthoGraphicMatrix();
            glm::mat4 view = ocsSys->camera->GetViewMatrix();
            shaderUsed->setMat4("projection",ortho);
            shaderUsed->setMat4("view", view);

            for (Entity* ent : m_sketch.get()->entities)
            {
                shaderUsed->setVec4("PaintColor",ent->color);
                ent->Paint();
            }

            if (isMainCanvas)
            {
                DrawTickers();
            }

            glfwSwapBuffers(m_window.get());
            glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_windowbuf);
        }
    }

    void Canvas::DrawTickers()
    {

    }
}