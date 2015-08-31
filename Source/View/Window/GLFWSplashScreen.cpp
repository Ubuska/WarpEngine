#include "GLFWSplashScreen.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Program.h"
#include "Bitmap.h"
#include "Texture.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GLFWwindow* Splash = NULL;

Texture* gTexture = NULL;




Program* gProgram = NULL;
GLuint gVAO = 0;
GLuint gVBO = 0;


void LoadSplashTexture() 
{
    Bitmap bmp = Bitmap::bitmapFromFile("/Users/petergubin/Desktop/Desktop\ Development/WarpEngine/WarpEngineSplash.png");
    bmp.flipVertically();
    gTexture = new Texture(bmp);
}


// loads the vertex shader and fragment shader, and links them to make the global gProgram
void LoadSplashShaders() 
{
    std::vector<Shader> shaders;
	shaders.push_back(Shader::ShaderFromFile("/Users/petergubin/Desktop/Desktop\ Development/WarpEngine/splash-vertex-shader.txt", GL_VERTEX_SHADER));
	shaders.push_back(Shader::ShaderFromFile("/Users/petergubin/Desktop/Desktop\ Development/WarpEngine/splash-fragment-shader.txt", GL_FRAGMENT_SHADER));
    gProgram = new Program(shaders);
}


// loads a full screen quad into the VAO global
void LoadFullScreenQuad() 
{

    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    // Put the three triangle verticies into the VBO
    GLfloat vertexData[] = 
	{

       -1.0f,  1.0f,	// v0 - top left corner
		-1.0f, -1.0f,	// v1 - bottom left corner
		 1.0f,  1.0f,	// v2 - top right corner
		 1.0f, -1.0f	// v3 - bottom right corner
    };

	// Create the vertex array object for the full screen quad.
	
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);


	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(gProgram->attrib("vertTexCoord"));
	glVertexAttribPointer(gProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLFWSplashScreen::GLFWSplashScreen(void)
{

}


bool GLFWSplashScreen::VInitializeInstance(int _ResolutionX, int _ResolutionY, const char* _Name)
{
	WindowName = _Name;

    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

	 // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, 0);

	Splash = glfwCreateWindow(_ResolutionX, _ResolutionY, WindowName, NULL, NULL);

	glfwSetInputMode(Splash, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPos(Splash, 0, 0);
	glfwSetWindowPos(Splash, 1920/2 - _ResolutionX/2, 1080/2 - _ResolutionY/2);
    glfwMakeContextCurrent(Splash);
	

    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // GLEW throws some errors, so discard all the errors so far
    while(glGetError() != GL_NO_ERROR) {}

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
	{
        throw std::runtime_error("OpenGL 3.2 API is not available.");
		return false;
	}

	LoadSplashShaders();
	LoadFullScreenQuad();
	LoadSplashTexture();

	return true;
}

void GLFWSplashScreen::VOnUpdate(float _DeltaTime)
{

}

void GLFWSplashScreen::VOnRender()
{
	 // swap the display buffers (displays what was just drawn)
	// clear everything
    glClearColor(0, 0, 0, 1); // black

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // bind the program (the shaders)
	gProgram->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexture->object());
	gProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
        
    // bind the VAO (the triangle)
    glBindVertexArray(gVAO);
    
    // draw the VAO
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
    
    // unbind the VAO
    glBindVertexArray(0);
    
    // unbind the program
    glUseProgram(0);
	glfwSwapBuffers(Splash);
}

bool GLFWSplashScreen::VCloseWindow()
{

	delete gProgram;
	
	 glfwTerminate();
	return true;
}