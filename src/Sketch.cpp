#include "Sketch.h"
#include "MathUtils.h"
#include "DrawEntity.h"
#include "Canvas.h"

std::vector<glm::vec3> vertexs;
std::vector<glm::vec3> splineControls;
std::vector<float> splineKnots;
int vertexSize;
int knotsSize;
bool polylineClose;

DeepNestCpp::Sketch::Sketch()
{

}

DeepNestCpp::Sketch::~Sketch()
{
}

void DeepNestCpp::Sketch::OnLoadArc(const DL_ArcData& data)
{
	Arc2D* arc = new Arc2D(glm::vec3(data.cx,data.cy,0.0f),data.radius,data.angle1*deg2Rad,data.angle2*deg2Rad);
	entities.push_back(arc);
	mainCanvas->addEntity(arc);
}

void DeepNestCpp::Sketch::OnLoadCircle(const DL_CircleData& data)
{
	Circle2D* circle = new Circle2D(glm::vec3(data.cx, data.cy, 0.0f), data.radius);
	entities.push_back(circle);
	mainCanvas->addEntity(circle);
}

void DeepNestCpp::Sketch::OnLoadEllipse(const DL_EllipseData& data)
{
	float radiusX = glm::length(glm::vec3(data.mx, data.my, 0.0f));
	Ellipse2D* ellipse = new Ellipse2D(glm::vec3(data.cx, data.cy, 0.0f), radiusX, radiusX * data.ratio);
	entities.push_back(ellipse);
	mainCanvas->addEntity(ellipse);
}

void DeepNestCpp::Sketch::OnLoadLine(const DL_LineData& data)
{
	Line2D* line = new Line2D(glm::vec3(data.x1, data.y1, 0.0f), glm::vec3(data.x2, data.y2,0.0f));
	entities.push_back(line);
	mainCanvas->addEntity(line);
}

void DeepNestCpp::Sketch::OnLoadPoint(const DL_PointData& data)
{
	Point2D* point = new Point2D(glm::vec3(data.x,data.y,0.0f));
	entities.push_back(point);
	mainCanvas->addEntity(point);
}

void DeepNestCpp::Sketch::OnLoadControlPoint(const DL_ControlPointData& data)
{
	splineControls.push_back(glm::vec3(data.x, data.y, 0.0f));
	vertexSize--;
	if (vertexSize == 0)
	{
		Spline2D* spline = new Spline2D(splineControls,splineKnots);
		entities.push_back(spline);
		mainCanvas->addEntity(spline);
	}
}

void DeepNestCpp::Sketch::OnLoadKnot(const DL_KnotData& data)
{
	splineKnots.push_back(data.k);

	knotsSize--;
}

void DeepNestCpp::Sketch::onLoadSpline(const DL_SplineData& data)
{
	splineControls.clear();
	splineKnots.clear();
	vertexSize = data.nControl;
	knotsSize = data.nFit;
}

void DeepNestCpp::Sketch::OnLoadPolyline(const DL_PolylineData& data)
{
	vertexSize = data.number;
	vertexs.clear();
	polylineClose = data.flags;
}

void DeepNestCpp::Sketch::OnLoadVertex(const DL_VertexData& data)
{
	vertexs.push_back(glm::vec3(data.x, data.y, 0.0f));
	vertexSize--;
	if (vertexSize == 0)
	{
		Polyline2D* polyline = new Polyline2D(vertexs,polylineClose);
		entities.push_back(polyline);
		mainCanvas->addEntity(polyline);
	}
}


void DeepNestCpp::Sketch::UpdateSketch()
{
	mainCanvas->UpdateOCS();
	for (Canvas* canvas : partPreview)
	{
		canvas->UpdateOCS();
	}
}
