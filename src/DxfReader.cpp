#include "DxfReader.h"
#include "DrawEntity.h"
#include "MathUtils.h"
#include "Sketch.h"

std::vector<glm::vec3> vertexs;
std::vector<glm::vec3> splineControls;
std::vector<float> splineKnots;
int vertexSize;
int knotsSize;
bool polylineClose;

namespace DeepNestCpp
{
	DxfReader::DxfReader(Sketch* sketch,const std::string &dxfFileName) : sketch(sketch)
	{
		
		DL_Dxf* dxfHandle = new DL_Dxf();
		if (!dxfHandle->in(dxfFileName,this))
		{
			std::cout << "read Dxf Failed" << std::endl;
		}

		delete dxfHandle;
	}
	DxfReader::~DxfReader()
	{
	}
	void DxfReader::addLayer(const DL_LayerData& data)
	{
	}
	void DxfReader::addLinetype(const DL_LinetypeData& data)
	{
	}
	void DxfReader::addLinetypeDash(double data)
	{
	}
	void DxfReader::addBlock(const DL_BlockData& data)
	{
	}
	void DxfReader::endBlock()
	{

	}
	void DxfReader::addTextStyle(const DL_StyleData& data)
	{
	}
	void DxfReader::addPoint(const DL_PointData& data)
	{
		Point2D* point = new Point2D(glm::vec3(data.x, data.y, 0.0f));
		sketch->AddEntity(point);
	}
	void DxfReader::addLine(const DL_LineData& data)
	{
		Line2D* line = new Line2D(glm::vec3(data.x1, data.y1, 0.0f), glm::vec3(data.x2, data.y2, 0.0f));
		sketch->AddEntity(line);
	}
	void DxfReader::addXLine(const DL_XLineData& data)
	{
	}
	void DxfReader::addRay(const DL_RayData& data)
	{
	}
	void DxfReader::addArc(const DL_ArcData& data)
	{
		Arc2D* arc = new Arc2D(glm::vec3(data.cx, data.cy, 0.0f), data.radius, data.angle1 * deg2Rad, data.angle2 * deg2Rad);
		sketch->AddEntity(arc);
	}
	void DxfReader::addEllipse(const DL_EllipseData& data)
	{
		float radiusX = glm::length(glm::vec3(data.mx, data.my, 0.0f));
		Ellipse2D* ellipse = new Ellipse2D(glm::vec3(data.cx, data.cy, 0.0f), radiusX, radiusX * data.ratio);
		sketch->AddEntity(ellipse);
	}
	void DxfReader::addCircle(const DL_CircleData& data)
	{
		Circle2D* circle = new Circle2D(glm::vec3(data.cx, data.cy, 0.0f), data.radius);
		sketch->AddEntity(circle);
	}
	void DxfReader::addPolyline(const DL_PolylineData& data)
	{
		vertexSize = data.number;
		vertexs.clear();
		polylineClose = data.flags;
	}
	void DxfReader::addVertex(const DL_VertexData& data)
	{
		vertexs.push_back(glm::vec3(data.x, data.y, 0.0f));
		vertexSize--;
		if (vertexSize == 0)
		{
			Polyline2D* polyline = new Polyline2D(vertexs, polylineClose);
			sketch->AddEntity(polyline);
		}
	}
	void DxfReader::addSpline(const DL_SplineData& data)
	{
		splineControls.clear();
		splineKnots.clear();
		vertexSize = data.nControl;
		knotsSize = data.nFit;
	}
	void DxfReader::addControlPoint(const DL_ControlPointData& data)
	{
		splineControls.push_back(glm::vec3(data.x,data.y,0.0f));
		vertexSize--;
		if (vertexSize == 0)
		{
			Spline2D* spline = new Spline2D(splineControls, splineKnots, false);
			sketch->AddEntity(spline);
		}
	}
	void DxfReader::addFitPoint(const DL_FitPointData& data)
	{
	}
	void DxfReader::addKnot(const DL_KnotData& data)
	{
		splineKnots.push_back(data.k);
		knotsSize--;
	}
	void DxfReader::addInsert(const DL_InsertData& data)
	{
	}
	void DxfReader::addMText(const DL_MTextData& data)
	{
	}
	void DxfReader::addText(const DL_TextData& data)
	{
	}
	void DxfReader::addArcAlignedText(const DL_ArcAlignedTextData& data)
	{
	}
	void DxfReader::addAttribute(const DL_AttributeData& data)
	{
	}
	void DxfReader::addDimAlign(const DL_DimensionData& dimData, const DL_DimAlignedData& dimAlignData)
	{
	}
	void DxfReader::addDimLinear(const DL_DimensionData& dimData, const DL_DimLinearData& dimLinearData)
	{
	}
	void DxfReader::addDimRadial(const DL_DimensionData&, const DL_DimRadialData&)
	{
	}
	void DxfReader::addDimDiametric(const DL_DimensionData&, const DL_DimDiametricData&)
	{
	}
	void DxfReader::addDimAngular(const DL_DimensionData&, const DL_DimAngular2LData&)
	{
	}
	void DxfReader::addDimAngular3P(const DL_DimensionData&, const DL_DimAngular3PData&)
	{
	}
	void DxfReader::addDimOrdinate(const DL_DimensionData&, const DL_DimOrdinateData&)
	{
	}
	void DxfReader::addComment(const std::string& comment)
	{
	}
	void DxfReader::addLeader(const DL_LeaderData& data)
	{
	}
	void DxfReader::addLeaderVertex(const DL_LeaderVertexData& data)
	{
	}
	void DxfReader::addHatch(const DL_HatchData& data)
	{
	}
	void DxfReader::addTrace(const DL_TraceData& data)
	{
	}
	void DxfReader::add3dFace(const DL_3dFaceData& data)
	{
	}
	void DxfReader::addSolid(const DL_SolidData& data)
	{
	}
	void DxfReader::addImage(const DL_ImageData& data)
	{
	}
	void DxfReader::linkImage(const DL_ImageDefData& data)
	{
	}
	void DxfReader::addHatchLoop(const DL_HatchLoopData& data)
	{
	}
	void DxfReader::addHatchEdge(const DL_HatchEdgeData& data)
	{
	}
	void DxfReader::addXRecord(const std::string& record)
	{
	}
	void DxfReader::addXRecordString(int index, const std::string& record)
	{
	}
	void DxfReader::addXRecordReal(int index, double data)
	{
	}
	void DxfReader::addXRecordInt(int index, int data)
	{
	}
	void DxfReader::addXRecordBool(int index, bool data)
	{
	}
	void DxfReader::addXDataApp(const std::string& app)
	{
	}
	void DxfReader::addXDataString(int index, const std::string& data)
	{
	}
	void DxfReader::addXDataReal(int index, double real)
	{
	}
	void DxfReader::addXDataInt(int index, int data)
	{
	}
	void DxfReader::addDictionary(const DL_DictionaryData& data)
	{
	}
	void DxfReader::addDictionaryEntry(const DL_DictionaryEntryData& data)
	{
	}
}