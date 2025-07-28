#version 420 core
layout (location = 0) in vec2 aPosition;

// //map (-1,1) ndc coordinate to (0,0,window.Width,window.Height)
// uniform int windowWidth;
// uniform int windowHeight;

// uniform float scaleDrawingFitCanvas;
// uniform float xoffset;
// uniform float yoffset;

// #define HalfWidth windowWidth/2
// #define HalfHeight windowHeight/2

// void main()
// {
//     vec2 fitedPosition = aPosition/scaleDrawingFitCanvas;
//     float xPosition = fitedPosition.x - xoffset;
//     float yPosition = fitedPosition.y - yoffset;
//     vec2 transformedPos = vec2((xPosition - HalfWidth)/(HalfWidth)+1.0f,(yPosition - HalfHeight)/(HalfHeight)+1.0f);
//     gl_Position = vec4(transformedPos,0.0, 1.0);
// }

uniform mat4 projection;
uniform mat4 view;
void main()
{
    gl_Position = projection * view * vec4(aPosition,0.0f,1.0f);
}
