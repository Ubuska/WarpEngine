#include "GLFWWindow.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Core/Application.h"

const float MouseSensitivity = 0.1f;
double mouseX, mouseY;
double gScrollY = 0.0;

GLFWwindow *Self = NULL;

void OnScroll(GLFWwindow* _Window, double _DeltaX, double _DeltaY) 
{
  gScrollY += _DeltaY;
}

GLFWWindow::GLFWWindow(void)
{

}


bool GLFWWindow::VInitializeInstance(int _ResolutionX, int _ResolutionY, const char* _Name)
{
	WindowName = _Name;

// initialise GLFW
    //glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

	 // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	// Non resizeable window
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Resizeable window
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, 1);

	



	// Fullscreen mode
	//gp_Window = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "OpenGL Tutorial", glfwGetPrimaryMonitor(), NULL);
	
	// Window Mode
	//Self = glfwCreateWindow((int)m_ScreenSize.x, (int)m_ScreenSize.y, "OpenGL Tutorial", NULL, NULL);
	Self = glfwCreateWindow(_ResolutionX, _ResolutionY, WindowName, NULL, NULL);
	//if(!m_pMainWindow)
    //    throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");

    // GLFW settings
	//glfwSetInputMode(gp_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetInputMode(Self, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(Self, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(Self, 0, 0);
	glfwSetWindowPos(Self, 1920/2 - _ResolutionX/2, 1080/2 - _ResolutionY/2);
    glfwMakeContextCurrent(Self);
	

	glfwSetScrollCallback(Self, OnScroll);

    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // GLEW throws some errors, so discard all the errors so far
    while(glGetError() != GL_NO_ERROR) {}
	

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
	{
        throw std::runtime_error("OpenGL 3.2 API is not available.");
		return false;
	}

    // OpenGL settings
	
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	return true;
}


float X;


void GLFWWindow::VOnUpdate(float _DeltaTime)
{

	if (glfwWindowShouldClose(Self))
	{
		VCloseWindow();
		return ;
	}
	// TEMPORARY INPUT HERE
	//move position of camera based on WASD keys
    const float moveSpeed = 2.0; //units per second
    if(glfwGetKey(Self, 'S'))
	{
		
		m_pHumanView->m_pCamera->OffsetPosition(_DeltaTime * moveSpeed * -m_pHumanView->m_pCamera->Forward());
		
	} 
	else if(glfwGetKey(Self, 'W'))
	{
		m_pHumanView->m_pCamera->OffsetPosition(_DeltaTime * moveSpeed * m_pHumanView->m_pCamera->Forward());
        //gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.forward());
    }
    if(glfwGetKey(Self, 'A'))
	{
		m_pHumanView->m_pCamera->OffsetPosition(_DeltaTime * moveSpeed * -m_pHumanView->m_pCamera->Right());
      // gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.right());
    }
	else if(glfwGetKey(Self, 'D'))
	{
		m_pHumanView->m_pCamera->OffsetPosition(_DeltaTime * moveSpeed * m_pHumanView->m_pCamera->Right());
       // gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.right());
    }
	if(glfwGetKey(Self, 'Z'))
	{
		m_pHumanView->m_pCamera->OffsetPosition(_DeltaTime * moveSpeed * glm::vec3(0,1,0));
	} 
	if(glfwGetKey(Self, 'Q'))
	{
		mat4x4 T = mat4x4();
		X += 0.5f;
		T = translate(mat4x4(), vec3(X, X, X));
		g_pApp->m_pGame->VCreateActor("actors\\testactor.xml", NULL, &T, NULL);
	} 
	if(glfwGetKey(Self, 'E'))
	{
		
		g_pApp->m_pGame->VDestroyActor(g_pApp->m_pGame->GetLastCreatedActorId());
	} 
	else if(glfwGetKey(Self, 'X'))
	{
		m_pHumanView->m_pCamera->OffsetPosition(_DeltaTime * moveSpeed * -glm::vec3(0,1,0));
	}
	if (glfwGetKey(Self, GLFW_KEY_LEFT_ALT))
	{
		if (glfwGetKey(Self, GLFW_KEY_ENTER))
		{
			glfwSetWindowSize(Self, 1920, 1080);
			glfwSetWindowPos(Self, 0, 0);

		}
	}

	//rotate camera based on mouse movement
	
	
	glfwGetCursorPos(Self, &mouseX, &mouseY);
	m_pHumanView->m_pCamera->OffsetOrientation(MouseSensitivity * (float)mouseY, MouseSensitivity * (float)mouseX);
	glfwSetCursorPos(Self, 0, 0); //reset the mouse, so it doesn't go out of the window

	const float zoomSensitivity = -1.2f;
	float fieldOfView = m_pHumanView->m_pCamera->GetFieldOfView() + zoomSensitivity * (float)gScrollY;
	if(fieldOfView < 5.0f) fieldOfView = 5.0f;
	if(fieldOfView > 130.0f) fieldOfView = 130.0f;
	m_pHumanView->m_pCamera->SetFieldOfView(fieldOfView);
	gScrollY = 0;
}

void GLFWWindow::VOnRender()
{
	
	 // swap the display buffers (displays what was just drawn)
	glfwSwapBuffers(Self);
}

bool GLFWWindow::VCloseWindow()
{
	// We need to tell our Renderer that we closing this window
	//glfwTerminate();
	return false;
}

