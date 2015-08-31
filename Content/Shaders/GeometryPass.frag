#version 330 



uniform sampler2D tex;

in vec2 TexCoord0;
in vec3 WorldPos0;
in vec3 Normal0;   

layout (location = 0) out vec4 WorldPosOut;  
layout (location = 1) out vec4 DiffuseOut; 
layout (location = 2) out vec4 NormalOut;     
layout (location = 3) out vec4 TexCoordOut;  

void main() 
{


	WorldPosOut = vec4(WorldPos0, 1);
	DiffuseOut = texture(tex, TexCoord0);
	NormalOut = vec4(normalize(Normal0),1);
	TexCoordOut = vec4(TexCoord0, 0.0, 1.0);		
    //note: the texture function was called texture2D in older versions of GLSL
    //finalColor = texture(tex, TexCoord0);
}