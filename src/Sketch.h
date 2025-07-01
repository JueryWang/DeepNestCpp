#pragma once
#include <vector>
#include "../lib/dxflib/include/dl_entities.h"

namespace DeepNestCpp
{
	class Canvas;
	class Entity;

	class Sketch
	{
	public:
		Sketch();
		~Sketch();

		void OnLoadArc(const DL_ArcData& data);
		void OnLoadCircle(const DL_CircleData& data);
		void OnLoadEllipse(const DL_EllipseData& data);
		void OnLoadLine(const DL_LineData& data);
		void OnLoadPoint(const DL_PointData& data);
		void OnLoadPolyline(const DL_PolylineData& data);
		void OnLoadControlPoint(const DL_ControlPointData& data);
		void onLoadSpline(const DL_SplineData& data);
		void OnLoadVertex(const DL_VertexData& data);
		void OnLoadKnot(const DL_KnotData& data);
		void UpdateSketch();
	private:

		EntityType entType;
		Canvas* mainCanvas;
		std::vector<Canvas*> partPreview;
		std::vector<Entity*> entities;
	};
}
