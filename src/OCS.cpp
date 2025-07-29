#include "OCS.h"
#include "AABB.h"
#include "MathUtils.h"
#include "Sketch.h"
#include <cmath>

namespace DeepNestCpp
{
    OCS::OCS(std::shared_ptr<Sketch> sketch) : sketch(sketch)
    {
        
    }
    OCS::~OCS()
    {
        sketch.reset();
        delete camera;
    }

    void OCS::SetCanvasSizae(int width, int height)
    {
        canvasWidth = width;
        canvasHeight = height;
    }

    void OCS::ComputeScaleFitToCanvas()
    {
        float fitRatio = 1.0f/2.0f;
        if(sketch && sketch->entities.size()> 0)
        {
            objectRange = new AABB((sketch->entities[0]->bbox));

            for (int i = 1; i < sketch->entities.size(); i++)
            {
                objectRange->Union(sketch->entities[i]->bbox);
            }

            glm::vec3 canvasCenter = objectRange->Center();
            glm::vec3 canvasLB;
            glm::vec3 canvasRT;
            float widthExpand = objectRange->XRange() / canvasWidth;
            float heightExpand = objectRange->YRange() / canvasHeight;
            float maxExpand = std::max(widthExpand, heightExpand);

            canvasLB = glm::vec3(canvasCenter.x - (maxExpand * canvasWidth) / 2 * 1 / fitRatio, canvasCenter.y - (maxExpand * canvasHeight) / 2 * 1 / fitRatio,0.0f);
            canvasRT = glm::vec3(canvasCenter.x + (maxExpand * canvasWidth) / 2 * 1 / fitRatio, canvasCenter.y + (maxExpand * canvasHeight) / 2 * 1 / fitRatio,0.0f);

            canvasLB *= scale;
            canvasRT *= scale;
            canvasRange = new AABB(canvasLB,canvasRT);

            glm::vec3 centerCanvas = canvasRange->Center();
            glm::vec3 centerObj = objectRange->Center();
            canvasRange->Translate(centerObj - centerCanvas);
        }
        else
        {
            objectRange = new AABB(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(canvasWidth * fitRatio * scale,canvasHeight * fitRatio * scale,0.0f));
            glm::vec3 canvasCenter = objectRange->Center();
            glm::vec3 canvasLB = glm::vec3(canvasCenter.x - objectRange->XRange() / 2 * 1 / fitRatio, canvasCenter.x - objectRange->YRange() / 2 * 1 / fitRatio,0.0f);
            glm::vec3 canvasRT = glm::vec3(canvasCenter.x + objectRange->XRange() / 2 * 1 / fitRatio, canvasCenter.y + objectRange->YRange() / 2 * 1 / fitRatio, 0.0f);
            canvasLB *= scale;
            canvasRT *= scale;
            canvasRange = new AABB(canvasLB, canvasRT);
        }

        if(camera == nullptr)
        {
            camera = new Camera2D(canvasRange);
        }else
        {
            camera->UpdateRange(canvasRange);
        }
    }
    void OCS::UpdateTickers()
    {
        tickers.clear();

        float xrange = canvasRange->XRange();
        float yrange = canvasRange->YRange();
        int xpower = 0;
        int ypower = 0;

        int xdigit = MathUtils::GetFirstNoneZeroDigit(xrange/25.0f,xpower);
        int ydigit = MathUtils::GetFirstNoneZeroDigit(yrange/25.0f,ypower);

        if(xdigit > 8)
            XTickUnit = 8 * pow(10,xpower);
        else if(xdigit > 6)
            XTickUnit = 6 * pow(10,xpower);
        else if(xdigit > 4)
            XTickUnit = 4 * pow(10,xpower);
        else if (xdigit > 2)
            XTickUnit = 2 * pow(10,xpower);
        else 
            XTickUnit = 1 * pow(10,xpower);

        if(ydigit > 8)
            YTickUnit = 8 * pow(10,xpower);
        else if(xdigit > 6)
            YTickUnit = 6 * pow(10,xpower);
        else if(xdigit > 4)
            YTickUnit = 4 * pow(10,xpower);
        else if (xdigit > 2)
            YTickUnit = 2 * pow(10,xpower);
        else 
            YTickUnit = 1 * pow(10,xpower);
        
        float divisorX = (5 * XTickUnit);
        float divisorY = (5 * YTickUnit);

        float beginX = canvasRange->min.x;
        float beginY = canvasRange->min.y;
        float endX = canvasRange->max.x;
        float endY = canvasRange->max.y;

        beginX = beginX + fmod(beginX,XTickUnit);
        beginX = MathUtils::SmallestCeilling(beginX,xpower);

        while(beginX < endX)
        {
            float remainder = fmod(beginX,divisorX);
            bool isMainAxis = abs(remainder) < XTickUnit;
            glm::vec3 lineStart = glm::vec3(2*(beginX - canvasRange->min.x)/canvasRange->XRange() - 1.0f + 0.08f,-1.0f,0.0f);
            glm::vec3 lineEnd = glm::vec3(lineStart.x,isMainAxis ? -0.94f : -0.97f,0.0f);
            std::vector<glm::vec3> tickLine = {lineStart,lineEnd};
            AxisTicker tick = AxisTicker(beginX,tickLine,isMainAxis ? TickType::Main : TickType::Sub, AxisType::X);
            beginX += XTickUnit;
        }

        beginY = beginY + abs(beginY) < YTickUnit;
        beginY = MathUtils::SmallestCeilling(beginY,ypower);

        while(beginY < endY)
        {
            float remainder = fmod(beginY,divisorY);
            bool isMainAxis = abs(remainder) < YTickUnit;
            glm::vec3 lineStart = glm::vec3(-1.0f,2*(beginY - canvasRange->min.y)/canvasRange->YRange() - 1.0f + 0.08f,0.0f);
            glm::vec3 lineEnd = glm::vec3(isMainAxis ? -0.94f : -0.97f, lineStart.y,0.0f);
            std::vector<glm::vec3> tickLine = {lineStart,lineEnd};
            AxisTicker tick = AxisTicker(beginX,tickLine,isMainAxis ? TickType::Main : TickType::Sub, AxisType::Y);
            beginY += YTickUnit;
        }
    }

    void OCS::OnMouseScroll(float delta,const glm::vec2 &mousePosition)
    {
        glm::vec2 preOcsPos = GetOCSPosWithPixelPos(mousePosition);
        float scalar = delta > 0 ? zoomFactor : 1.0f / zoomFactor;
        glm::vec3 offset = glm::vec3(preOcsPos,0.0f) - canvasRange->Center();
        canvasRange->Translate(offset);
        canvasRange->Multiply(scalar);
        canvasRange->Translate(-offset * scalar);
        camera->UpdateRange(canvasRange);

        UpdateTickers();
    }

    void OCS::OnMouseMove(const glm::vec2 &offset)
    {
        glm::vec3 canvasOffset;
        canvasOffset.x = offset.x / canvasWidth * canvasRange->XRange();
        canvasOffset.y = offset.y / canvasHeight * canvasRange->YRange();
        canvasRange->Translate(canvasOffset);
        glm::vec3 center = canvasRange->Center();

        camera->UpdateRange(canvasRange);

        UpdateTickers();
    }

    glm::vec2 OCS::GetOCSPosWithPixelPos(const glm::vec2 &pixelPos)
    {
        float mouseXfraction = pixelPos.x / canvasWidth;
        float mouseYfraction = 1 - pixelPos.y / canvasHeight;

        return glm::vec3(canvasRange->min.x + mouseXfraction * canvasRange->XRange(),canvasRange->min.y + mouseYfraction * canvasRange->YRange(),0.0f);
    }
}
