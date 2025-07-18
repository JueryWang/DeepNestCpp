#include "OCS.h"
#include "AABB.h"
#include "MathUtils.h"
#include <cmath>

namespace DeepNestCpp
{
    OCS::OCS()
    {

    }
    OCS::~OCS()
    {
        delete objectRange;
        delete canvasRange;
        delete camera;
    }

    void OCS::addEntity(Entity* ent)
    {
        if(objectRange == nullptr)
        {
            objectRange = ent->aabb;
        }
        else
        {
            objectRange->Union(ent->aabb);
        }

        auto find = std::find(entityReference.begin(),entityReference.end(),ent);
        if(find == entityReference.end())
        {
            entityReference.push_back(ent);
        }
    }
    
    void OCS::deleteEntity(Entity* ent)
    {
        auto find = std::find(entityReference.begin(),entityReference.end(),ent);
        if(find != entityReference.end())
        {
            entityReference.erase(find);
        }
    }

    void OCS::SetCanvasSizae(int width, int height)
    {
        canvasWidth = width;
        canvasHeight = height;
    }

    void OCS::ComputeScaleFitToCanvas()
    {
        float fitRatio = 3.0f/4.0f;
        if(entityReference.size() > 0)
        {
            if(objectRange)
            {
                delete objectRange;
                objectRange = new AABB(entityReference[0]->aabb);
            }else
            {
                for(int i = 0; i < entityReference.size(); i++)
                {
                    objectRange->Union(entityReference[i]->aabb);
                }

                glm::vec3 canvasCenter = objectRange->Center();
                glm::vec3 canvasLB = glm::vec3(canvasCenter.x - objectRange->XRange()/2 * 1.0f/fitRatio,canvasCenter.y - objectRange->YRange()/2 * 1.0f/fitRatio,0.0f);
                glm::vec3 canvasRT = glm::vec3(canvasCenter.x + objectRange->XRange()/2 * 1.0f/fitRatio,canvasCenter.y - objectRange->YRange()/2 * 1.0f/fitRatio,0.0f);
                canvasLB *= scale;
                canvasRT *= scale;
                if(canvasRange)
                    delete canvasRange;
                canvasRange = std::move(new AABB(canvasLB,canvasRT));
            }
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

        int xdigit = MathUitls::GetFirstNoneZeroDigit(xrange/25.0f,xpower);
        int ydigit = MathUitls::GetFirstNoneZeroDigit(yrange/25.0f,ypower);

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
        beginX = MathUitls::SmallestCeilling(beginX,xpower);

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
        beginY = MathUitls::SmallestCeilling(beginY,ypower);

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
        glm::vec2 preOcsPos = GetOCSPosWithPixelPos(mouseHoverPosition);
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
