#pragma once
#include "../lib/OGL/glad/glad.h"
#include "../lib/OGL/glm/glm.hpp"
#include <iostream>
#include <vector>

namespace DeepNestCpp
{
    enum TickType
    {
        Main,
        Sub
    };

    enum AxisType
    {
        X,
        Y
    };

    class AxisTicker
    {
        public:
            AxisTicker(float value,const std::vector<glm::vec3>& oglPosition,TickType Ttype,AxisType Atype) : 
            tickCoord(oglPosition),value(value),Ttype(Ttype),Atype(Atype)
            {
                glGenVertexArrays(1,&vao);
                glBindVertexArray(vao);
                
                glGenBuffers(GL_ARRAY_BUFFER,&vbo);
                glBindBuffer(GL_ARRAY_BUFFER,vbo);
                
                glBufferData(GL_ARRAY_BUFFER,oglPosition.size() * sizeof(glm::vec3),oglPosition.data(),GL_STATIC_DRAW);
                glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(glm::vec3),0);
                glEnableVertexAttribArray(0);
            }

            ~AxisTicker()
            {
                std::cout<<"delete AxisTicker"<<std::endl;
                
                if(vao)
                {
                    glDeleteVertexArrays(1,&vao);
                }
                if(vbo)
                {
                    glDeleteBuffers(1,&vbo);
                }
                glBindVertexArray(0);
            }

            void paint()
            {
                glBindVertexArray(vao);
                glDrawArrays(GL_LINES,0,2);
            }

        private:
            GLuint vao;
            GLuint vbo;
            float value;
            std::vector<glm::vec3> tickCoord;
            TickType Ttype;
            AxisType Atype;
    };
}