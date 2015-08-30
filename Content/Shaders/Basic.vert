#version 150

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec2 vertTexCoord;
in vec3 vertNormal;


out vec3 fragVert;
out vec3 fragNormal;

// Output data ; will be interpolated for each fragment.
out vec2 UV;



void main()
 {
    // Pass some variables to the fragment shader

    fragNormal = vertNormal;
    fragVert = vert;
    
    // Apply all matrix transformations to vert
    gl_Position = camera * model * vec4(vert, 1);

	// --------------------------------------------
	//vec3 vertexTangent_cameraspace = vec3(camera) * vertexTangent_modelspace;
	//vec3 vertexBitangent_cameraspace = vec3(camera) * vertexBitangent_modelspace;
	//vec3 vertexNormal_cameraspace = vec3(camera) * vertNormal;

	// UV of the vertex. No special space for this one.
	UV = vertTexCoord;
	
}