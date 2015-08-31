#version 330 

uniform mat4 camera;
uniform mat4 model;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
//layout (location = 2) in vec3 Normal;  

out vec2 TexCoord0;                                                                 
out vec3 Normal0;                                                                   
out vec3 WorldPos0;   

void main() 
{
    // Pass the tex coord straight through to the fragment shader
    TexCoord0 = TexCoord;
    
    Normal0 = (model * vec4(Position, 1.0)).xyz; 
    // Apply all matrix transformations to vert
    gl_Position = camera * model * vec4(Position, 1);
    WorldPos0 = Position;
}