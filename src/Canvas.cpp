#include "Canvas.h"
#include "DrawEntity.h"
#include "OCS.h"
#include <QMouseEvent>

namespace DeepNestCpp
{
    Shader Canvas::drawEntityShader = Shader("Resources/drawEntity.vert","Resources/drawEntity.frag");
    Shader Canvas::drawTickerShader = Shader("Resources/drawTick.vert", "Resources/drawTick.frag");
    Shader Canvas::drawTickerTextShader = Shader("Resources/drawTickText.vert", "Resources/drawTick.frag");

    Canvas::Canvas(int width,int height, bool isMainCanvas) : isMainCanvas(isMainCanvas)
    {
        firstResize = true;
        window = glfwCreateWindow(width,height,"",NULL,NULL);
        
        this->resize(width, height);
        glfwMakeContextCurrent(window);
    }

    Canvas::~Canvas()
    {

    }

    void Canvas::addEntity(Entity* ent)
    {
        entities.push_back(ent);
    }

    void Canvas::Paint()
    {
        glfwMakeContextCurrent(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float aspectRatio = (float)width / (float)height;
        drawEntityShader.setVec4("PaintColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        drawEntityShader.setMat4("projection", ocsSys->GetCamera()->GetOrthoGraphicMatrix(aspectRatio));
        drawEntityShader.setMat4("view", ocsSys->GetCamera()->GetViewMatrix());

        for (Entity* ent : part->entities)
        {
            ent->Paint();
        }

        if (isMainCanvas)
            DrawTickers();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
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
        if (obj == this)
        {
            switch (event->type())
            {
                case QEvent::Resize:
                {
                    int width, height;
                    glfwGetWindowSize(window, &width, &height);
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
                        this->Paint();
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
        }
    }

    void Canvas::DrawTickers()
    {

    }
}