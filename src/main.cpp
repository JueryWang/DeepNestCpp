#include <QApplication>
#include "Sketch.h"
#include "DxfReader.h"
#include "OpenGLContext.h"
#include "GLWidget.h"
#include "Canvas.h"

using namespace DeepNestCpp;
int main(int argc,char* argv[])
{
    QApplication app(argc, argv);
    DeepNestCpp::InitializeOpenGL();
    
    std::shared_ptr<Sketch> sketch(new Sketch);
    DXFProcessor processor(sketch);
    processor.read("C:/Users/Administrator/Desktop/Red-Bull-Black-Print.dxf");
    int width = 1000;
    int height = 800;
    DeepNestCpp::Canvas* canvasMain = new DeepNestCpp::Canvas(sketch,1000,800,true);
    GLWidget* glWidget = new GLWidget(canvasMain, nullptr, DYNAMIC_DRAW);
    glWidget->show();

	return app.exec();
}