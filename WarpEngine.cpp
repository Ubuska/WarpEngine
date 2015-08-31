// WarpEngine.cpp: определяет точку входа для консольного приложения.
//

#include <stdio.h>
//#include <tchar.h>

#include <iostream>

// third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "matrix_transform.hpp"

// Engine units
#include "WarpEngineStd.h"
#include "Logger.h"
//#include "Core/BaseGameLogic.h"
#include "Application.h"
#include "ActorFactory.h"


// globals


//WE::Engine* m_pEngine = NULL;

// Prototypes
void Initialize();
bool IsOnlyOneInstance();
bool CheckStorage();
bool CheckMemory();
int CheckCPUSpeed();

Application* g_pApp = NULL;

int main()
{
	
	//g_pApp->InitializeInstance();
	
	g_pApp = WE_NEW Application();
	if (NULL == g_pApp || (!g_pApp->VInitializeInstance() ))
		{
			printf ("The Application failed to start");
		}


	
	/*g_BaseGameLogic = WE_NEW BaseGameLogic;
	if (NULL == g_BaseGameLogic || (!g_BaseGameLogic->Initialize() ))
		{
			printf ("The data files are somehow screwed");
		}*/
	/*
	g_DataFiles = new DataFiles()l
		if (NULL == g_DataFiles) || (!g_DataFiles->Initialize() ))
		{
			printf ("The data files are somehow screwed");
		}
	*/

	// Main Loop
	// run while the window is open
	double lastTime = glfwGetTime();
	
	while(g_pApp->IsApplicationRunning())
	{
        // process pending events
        glfwPollEvents();

        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
		g_pApp->VOnUpdate((float)(thisTime - lastTime));
		//m_pEngine->EngineUpdate((float)(thisTime - lastTime));
        lastTime = thisTime;

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << std::endl;

		
        //exit program if escape key is pressed
		
    }

    // clean up and exit
    glfwTerminate();

	return 0;
}

void Initialize()
{
	

}


bool IsOnlyOneInstance()
{
	return false;
}

bool CheckStorage()
{
	return false;
}

bool CheckMemory()
{
	return false;
}
int CheckCPUSpeed()
{
	return 0;
}