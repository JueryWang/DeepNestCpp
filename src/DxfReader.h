#pragma once

#include "../lib/dxflib/include/dl_dxf.h"
#include "../lib/dxflib/include/dl_creationadapter.h"
#include <string>

namespace
{
	class Sketch;
}

namespace DeepNestCpp
{
	class DxfReader : public DL_CreationAdapter
	{
		public:
			DxfReader(Sketch* sketch,const std::string &dxfFileName);
			~DxfReader();

			virtual void addLayer(const DL_LayerData& data);
			virtual void addLinetype(const DL_LinetypeData& data);
			virtual void addLinetypeDash(double data);
			virtual void addBlock(const DL_BlockData& data);
			virtual void endBlock();
			virtual void addTextStyle(const DL_StyleData& data);
			virtual void addPoint(const DL_PointData& data);
			virtual void addLine(const DL_LineData& data);
			virtual void addXLine(const DL_XLineData& data);
			virtual void addRay(const DL_RayData& data);
			virtual void addArc(const DL_ArcData& data);
			virtual void addCircle(const DL_CircleData& data);
			virtual void addPolyline(const DL_PolylineData& data);
			virtual void addVertex(const DL_VertexData& data);
			virtual void addSpline(const DL_SplineData& data);
			virtual void addControlPoint(const DL_ControlPointData& data);
			virtual void addFitPoint(const DL_FitPointData& data);
			virtual void addKnot(const DL_KnotData& data);
			virtual void addInsert(const DL_InsertData& data);
			virtual void addMText(const DL_MTextData& data);
			virtual void addText(const DL_TextData& data);
			virtual void addArcAlignedText(const DL_ArcAlignedTextData& data);
			virtual void addAttribute(const DL_AttributeData& data);
			virtual void addDimAlign(const DL_DimensionData& dimData, const DL_DimAlignedData& dimAlignData);
			virtual void addDimLinear(const DL_DimensionData& dimData, const DL_DimLinearData& dimLinearData);
			virtual void addDimRadial(const DL_DimensionData&,
				const DL_DimRadialData&);
			virtual void addDimDiametric(const DL_DimensionData&,
				const DL_DimDiametricData&);
			virtual void addDimAngular(const DL_DimensionData&,
				const DL_DimAngular2LData&);
			virtual void addDimAngular3P(const DL_DimensionData&,
				const DL_DimAngular3PData&);
			virtual void addDimOrdinate(const DL_DimensionData&,
				const DL_DimOrdinateData&);
			virtual void addComment(const std::string& comment);
			virtual void addLeader(const DL_LeaderData& data);
			virtual void addLeaderVertex(const DL_LeaderVertexData& data);
			virtual void addHatch(const DL_HatchData& data);
			virtual void addTrace(const DL_TraceData& data);
			virtual void add3dFace(const DL_3dFaceData& data);
			virtual void addSolid(const DL_SolidData& data);
			virtual void addImage(const DL_ImageData& data);
			virtual void linkImage(const DL_ImageDefData& data);
			virtual void addHatchLoop(const DL_HatchLoopData& data);
			virtual void addHatchEdge(const DL_HatchEdgeData& data);
			virtual void addXRecord(const std::string& record);
			virtual void addXRecordString(int index, const std::string& record);
			virtual void addXRecordReal(int index, double data);
			virtual void addXRecordInt(int index, int data);
			virtual void addXRecordBool(int index, bool data);
			virtual void addXDataApp(const std::string& app);
			virtual void addXDataString(int index, const std::string& data);
			virtual void addXDataReal(int index, double real);
			virtual void addXDataInt(int index, int data);
			virtual void addDictionary(const DL_DictionaryData& data);
			virtual void addDictionaryEntry(const DL_DictionaryEntryData& data);
	};
}