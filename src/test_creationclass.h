#pragma once
#include "../lib/dxflib/include/dl_dxf.h"
#include "../lib/dxflib/include/dl_creationadapter.h"


class Test_CreationClass : public DL_CreationAdapter {
public:
    Test_CreationClass();

    virtual void addLayer(const DL_LayerData& data);
    virtual void addPoint(const DL_PointData& data);
    virtual void addLine(const DL_LineData& data);
    virtual void addArc(const DL_ArcData& data);
    virtual void addCircle(const DL_CircleData& data);
    virtual void addPolyline(const DL_PolylineData& data);
    virtual void addVertex(const DL_VertexData& data);
    virtual void add3dFace(const DL_3dFaceData& data);

    void printAttributes();
};
