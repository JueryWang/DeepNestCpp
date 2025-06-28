#pragma once
#include "DrawEntity.h"
#include "../lib/dxflib/include/dl_entities.h"

namespace DeepNestCpp
{
	class Sketch
	{
	public:
		Sketch();
		~Sketch();

		void OnLoadArc(const DL_ArcData& data);
		void OnLoadCircle(const DL_CircleData& data);
		void OnLoadLine(const DL_LineData& data);
		void OnLoadPoint(const DL_PointData& data);
		void OnLoadControlPoint(const DL_ControlPointData& data);
		void OnLoadPolyline(const DL_PolylineData& data);
		void onLoadSpline(const DL_SplineData& data);
	private:

		EntityType entType;
		int vertexSize;


	};
}
