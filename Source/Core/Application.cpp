#include "Application.h"

#include "ResourceCache/ResCache.h"

// Temporary?
#include "Processes/ProcessRender.h"
#include "Processes/ProcessDelay.h"
#include "Processes/ProcessEngineLoad.h"

#include "Events/TestSystem.h"
#include "Events/EventManager.h"
#include "Graphics3D/Geometry.h"
#include "Graphics3D/Renderer/OpenGLRenderer.h"
#include "View/HumanView.h"
#include "View/Window/GLFWWindow.h"
#include "View/Window/GLFWSplashScreen.h"

TestSystem* TS = NULL;
IWindow* m_pSplash = NULL;
HumanView* Human = NULL;
bool bIsEngineLoaded;

Application::Application(void)
{
	LOG("Application created");
	m_bIsApplicationRunning = true;
	g_pApp = this;
	m_pGame = NULL;
	m_pEventManager = NULL;
	m_pMainWindow = NULL;
	m_ResCache = NULL;

	m_ScreenSize = glm::vec2(640, 480);

}


Application::~Application(void)
{
	EventListenerDelegate DelegateFunction = fastdelegate::MakeDelegate(this, &Application::EngineLoadedDelegate);
	IEventManager::Get()->VRemoveListener(DelegateFunction, EventData_Engine_Loaded::sk_EventType);
}

bool Application::VInitializeInstance()
	{
		/*m_pGame = VCreateGameAndView();
		if (!m_pGame)
			return false;*/

		//
		// Initialize the ResCache - Chapter 5, page 141
		//
		//    Note - this is a little different from the book. Here we have a speccial resource ZIP file class, DevelopmentResourceZipFile,
		//    that actually reads directly from the source asset files, rather than the ZIP file. This is MUCH better during development, since
		//    you don't want to have to rebuild the ZIP file every time you make a minor change to an asset.
		//
		//IResourceFile *zipFile = (m_bIsEditorRunning || m_Options.m_useDevelopmentDirectories) ? 
			//WE_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor) :
		IResourceFile *ZipFile = WE_NEW ResourceZipFile(L"Assets.zip");

		m_ResCache = WE_NEW ResCache(50, ZipFile);

		if (!m_ResCache->Init())
		{
			printf("Failed to initialize resource cache!  Are your paths set up correctly?");
			return false;
		}

		//extern shared_ptr<IResourceLoader> CreateWAVResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateOGGResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateDDSResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateJPGResourceLoader();

		extern shared_ptr<IResourceLoader> CreateXMLResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateSdkMeshResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateScriptResourceLoader();

		// Note - register these in order from least specific to most specific! They get pushed onto a list.
		// RegisterLoader is discussed in Chapter 5, page 142
		//m_ResCache->RegisterLoader(CreateWAVResourceLoader());
		//m_ResCache->RegisterLoader(CreateOGGResourceLoader());
		//m_ResCache->RegisterLoader(CreateDDSResourceLoader());
		//m_ResCache->RegisterLoader(CreateJPGResourceLoader());
		m_ResCache->RegisterLoader(CreateXMLResourceLoader());
		//m_ResCache->RegisterLoader(CreateSdkMeshResourceLoader());
		//m_ResCache->RegisterLoader(CreateScriptResourceLoader());

		m_ProcessManager = new ProcessManager;
		m_pEventManager = WE_NEW EventManager("Warp Engine Generic Event Manager", true);

		EventListenerDelegate DelegateFunction = fastdelegate::MakeDelegate(this, &Application::EngineLoadedDelegate);
		IEventManager::Get()->VAddListener(DelegateFunction, EventData_Engine_Loaded::sk_EventType);

		TS = WE_NEW TestSystem();
		TS->Initialize();
		
		// Create a window
		//IWindow* Wind = new GLFWWindow();
		//Wind->VInitializeInstance(640, 400, "Main Window");

		// Splash Screen Creation
		m_pSplash = new GLFWSplashScreen();
		m_pSplash->VInitializeInstance(1024, 512, "Splash");

		// Renderer setup
		m_Renderer = shared_ptr<IRenderer>(WE_NEW OpenGLRenderer());
		//m_Renderer->SetBackgroundColor()

		// Temporary!
		m_pGame = WE_NEW BaseGameLogic;
		if (!m_pGame)
			return false;

		
		Human = WE_NEW HumanView(m_Renderer);
		m_pGame->VAddView(shared_ptr<IGameView>(Human));

		Human->SetWindow(m_pSplash);
		m_pSplash->VOnUpdate(1.0f);
		m_pSplash->VOnRender();
		//Human->SetWindow(Wind);

		StrongProcessPtr EngineLoad(new ProcessEngineLoad(3));
		m_ProcessManager->AttachProcess(EngineLoad);

		//m_Renderer->VAddRenderTarget(Wind);
		/*
		Mat4x4 Mat = Mat4x4();
		Mat.BuildRotationX(0.5f);
		Mat.BuildTranslation(2.0f, 3.0f, 10.0f);
		Mat.BuildTranslation(1.0f, 0.0f, 0.0f);
		Mat.BuildTranslation(1.0f, 0.0f, 0.0f);
		Mat.BuildTranslation(1.0f, 0.0f, 0.0f);
		Mat.BuildTranslation(1.0f, 0.0f, 0.0f);
		Mat.BuildTranslation(2.0f, 0.0f, 0.0f);
	
	StrongProcessPtr Render(new ProcessRender());

	StrongProcessPtr Delay(new ProcessDelay(10));
	StrongProcessPtr Delay2(new ProcessDelay(2));
	m_ProcessManager->AttachProcess(Delay);
	Delay->AttachChild(Delay2);
	Delay->AttachChild(Render);
	*/
		return true;
	}

void Application::VOnUpdate(float DeltaTime)
{
	//std::cout << DeltaTime << std::endl;
	m_ProcessManager->UpdateProcesses(DeltaTime);
	EventManager::Get()->VUpdate();

	m_pGame->VOnUpdate(DeltaTime, DeltaTime);
	// Render 
	BaseGameLogic *pGame = g_pApp->m_pGame;


	if (bIsEngineLoaded)
	{
	for(GameViewList::iterator i=pGame->m_GameViews.begin(),
		end=pGame->m_GameViews.end(); i!=end; ++i)
			{
				(*i)->VOnRender(DeltaTime, DeltaTime);
			}
	}


	//if(glfwWindowShouldClose(m_pMainWindow)) m_bIsApplicationRunning = false;
	//if(glfwGetKey(m_pMainWindow, GLFW_KEY_ESCAPE))
      //      glfwSetWindowShouldClose(m_pMainWindow, GL_TRUE);

	//if(glfwGetKey(m_pMainWindow, GLFW_KEY_0)) VCreateWindow();
}

bool Application::IsApplicationRunning()
{
	return m_bIsApplicationRunning;
}

void Application::EngineLoadedDelegate(IEventDataPtr _pEventData)
{
	m_pSplash->VCloseWindow();
	delete m_pSplash;
	bIsEngineLoaded = true;

	

	GLFWWindow* Window = WE_NEW GLFWWindow();
	Window->m_pHumanView = shared_ptr<HumanView>(Human);
	Human->SetWindow(Window);
	Window->VInitializeInstance(640, 480, "Warp Engine 0.1");

	m_pGame->Initialize();
	m_Renderer->VInitialize();

	m_Renderer->VAddRenderTarget(Window);

	// Temporary!
	Human->m_pCamera->SetPosition(vec3(0, 0, 4));
	Human->m_pCamera->SetViewportAspectRatio(640/480);

	

}
