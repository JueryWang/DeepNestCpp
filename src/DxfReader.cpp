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
	class DxfReader : public DL_CreationAdapter
	{
	public:
		DxfReader(std::weak_ptr<Sketch> sketch) : sketch(sketch)
		{
		}
		~DxfReader()
		{
		}
		virtual void addLayer(const DL_LayerData& data)
		{
		}
		virtual void addLinetype(const DL_LinetypeData& data)
		{
		}
		virtual void addLinetypeDash(double data)
		{
		}
		virtual void addBlock(const DL_BlockData& data)
		{
		}
		virtual void endBlock()
		{

		}
		virtual void addTextStyle(const DL_StyleData& data)
		{
		}
		virtual void addPoint(const DL_PointData& data)
		{
			Point2D* point = new Point2D(glm::vec3(data.x, data.y, 0.0f));
			sketch.lock()->AddEntity(point);
		}
		virtual void addLine(const DL_LineData& data)
		{
			Line2D* line = new Line2D(glm::vec3(data.x1, data.y1, 0.0f), glm::vec3(data.x2, data.y2, 0.0f));
			sketch.lock()->AddEntity(line);
		}
		virtual void addXLine(const DL_XLineData& data)
		{
		}
		virtual void addRay(const DL_RayData& data)
		{
		}
		virtual void addArc(const DL_ArcData& data)
		{
			Arc2D* arc = new Arc2D(glm::vec3(data.cx, data.cy, 0.0f), data.radius, data.angle1 * deg2Rad, data.angle2 * deg2Rad);
			sketch.lock()->AddEntity(arc);
		}
		virtual void addEllipse(const DL_EllipseData& data)
		{
			float radiusX = glm::length(glm::vec3(data.mx, data.my, 0.0f));
			Ellipse2D* ellipse = new Ellipse2D(glm::vec3(data.cx, data.cy, 0.0f), radiusX, radiusX * data.ratio);
			sketch.lock()->AddEntity(ellipse);
		}
		virtual void addCircle(const DL_CircleData& data)
		{
			Circle2D* circle = new Circle2D(glm::vec3(data.cx, data.cy, 0.0f), data.radius);
			sketch.lock()->AddEntity(circle);
		}
		virtual void addPolyline(const DL_PolylineData& data)
		{
			vertexSize = data.number;
			vertexs.clear();
			polylineClose = data.flags;
		}
		virtual void addVertex(const DL_VertexData& data)
		{
			vertexs.push_back(glm::vec3(data.x, data.y, 0.0f));
			vertexSize--;
			if (vertexSize == 0)
			{
				Polyline2D* polyline = new Polyline2D(vertexs, polylineClose);
				sketch.lock()->AddEntity(polyline);
			}
		}
		virtual void addSpline(const DL_SplineData& data)
		{
			splineControls.clear();
			splineKnots.clear();
			vertexSize = data.nControl;
			knotsSize = data.nFit;
		}
		virtual void addControlPoint(const DL_ControlPointData& data)
		{
			splineControls.push_back(glm::vec3(data.x, data.y, 0.0f));
			vertexSize--;
			if (vertexSize == 0)
			{
				Spline2D* spline = new Spline2D(splineControls, splineKnots, false);
				sketch.lock()->AddEntity(spline);
			}
		}
		virtual void addFitPoint(const DL_FitPointData& data)
		{
		}
		virtual void addKnot(const DL_KnotData& data)
		{
			splineKnots.push_back(data.k);
			knotsSize--;
		}
		virtual void addInsert(const DL_InsertData& data)
		{
		}
		virtual void addMText(const DL_MTextData& data)
		{
		}
		virtual void addText(const DL_TextData& data)
		{
		}
		virtual void addArcAlignedText(const DL_ArcAlignedTextData& data)
		{
		}
		virtual void addAttribute(const DL_AttributeData& data)
		{
		}
		virtual void addDimAlign(const DL_DimensionData& dimData, const DL_DimAlignedData& dimAlignData)
		{
		}
		virtual void addDimLinear(const DL_DimensionData& dimData, const DL_DimLinearData& dimLinearData)
		{
		}
		virtual void addDimRadial(const DL_DimensionData&, const DL_DimRadialData&)
		{
		}
		virtual void addDimDiametric(const DL_DimensionData&, const DL_DimDiametricData&)
		{
		}
		virtual void addDimAngular(const DL_DimensionData&, const DL_DimAngular2LData&)
		{
		}
		virtual void addDimAngular3P(const DL_DimensionData&, const DL_DimAngular3PData&)
		{
		}
		virtual void addDimOrdinate(const DL_DimensionData&, const DL_DimOrdinateData&)
		{
		}
		virtual void addComment(const std::string& comment)
		{
		}
		virtual void addLeader(const DL_LeaderData& data)
		{
		}
		virtual void addLeaderVertex(const DL_LeaderVertexData& data)
		{
		}
		virtual void addHatch(const DL_HatchData& data)
		{
		}
		virtual void addTrace(const DL_TraceData& data)
		{
		}
		virtual void add3dFace(const DL_3dFaceData& data)
		{
		}
		virtual void addSolid(const DL_SolidData& data)
		{
		}
		virtual void addImage(const DL_ImageData& data)
		{
		}
		virtual void linkImage(const DL_ImageDefData& data)
		{
		}
		virtual void addHatchLoop(const DL_HatchLoopData& data)
		{
		}
		virtual void addHatchEdge(const DL_HatchEdgeData& data)
		{
		}
		virtual void addXRecord(const std::string& record)
		{
		}
		virtual void addXRecordString(int index, const std::string& record)
		{
		}
		virtual void addXRecordReal(int index, double data)
		{
		}
		virtual void addXRecordInt(int index, int data)
		{
		}
		virtual void addXRecordBool(int index, bool data)
		{
		}
		virtual void addXDataApp(const std::string& app)
		{
		}
		virtual void addXDataString(int index, const std::string& data)
		{
		}
		virtual void addXDataReal(int index, double real)
		{
		}
		virtual void addXDataInt(int index, int data)
		{
		}
		virtual void addDictionary(const DL_DictionaryData& data)
		{
		}
		virtual void addDictionaryEntry(const DL_DictionaryEntryData& data)
		{
		}
		private:
			std::weak_ptr<Sketch> sketch;
	};


	DXFProcessor::DXFProcessor(std::shared_ptr<Sketch> sketch) : psketch(sketch)
	{
		
	}
	DXFProcessor::~DXFProcessor()
	{
	}
	int DXFProcessor::read(const std::string& dxfFile)
	{
		DxfReader* dxfReader = new DxfReader(psketch);
		DL_Dxf* dxf = new DL_Dxf();
		if (!dxf->in(dxfFile, dxfReader))
		{
			return 1;
		}

		delete dxf;
		delete dxfReader;

		return 0;
	}
}