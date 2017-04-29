////////////////////////////////////////////////////////////////////////////////
// Game.cpp

#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "ApConfig.h"
#include "BtTime.h"
#include "ShTouch.h"
#include "glfw.h"
#include "ErrorLog.h"
#include "RsUtil.h"
#include "RsImpl.h"
#include "GaProject.h"
#include "SdSoundWinGL.h"
#include "BtTimeDX11.h"
#include "UiInputWinGL.h"
#include "UiKeyboard.h"
#include "GaProject.h"
#include "gamemouse.h"
#include "ShJoystick.h"
#include "ShKeyboard.h"
#include "FsFile.h"

//#pragma comment(lib, "XInput.lib") // Library containing necessary 360

UiInputWinGL input;
GaProject *project;
BtBool isClosing = BtFalse;
GameMouse m_gameMouse;
BtFloat	m_frameRemainder;
BtFloat	m_lastGameLogic;
BtS32 m_width, m_height;
HANDLE g_mutex;

void GameUpdate();

////////////////////////////////////////////////////////////////////////////////
// windowCloseListener

int windowCloseListener()
{
	project->SetClosing();
	isClosing = BtTrue;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// GetDesktopResolution

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


////////////////////////////////////////////////////////////////////////////////
// Create

// static
void Game::Create(GaProject *pProject)
{
	ErrorLog::Filename("game.log");
	m_frameRemainder = BtTime::GetTick();

	BtTime::SetTick(1.0f / 60.0f);

	project = pProject;
	
	// Set the platform
	ApConfig::SetPlatform( ApPlatform_WinGL );
	ApConfig::SetDevice( ApDevice_WIN );

	GLFWvidmode dvm;

	// Count the number of instances of this program
	BtChar text[256];
	for (BtU32 i = 0; i < 10; i++)
	{
		sprintf(text, "NameOfMutexObject%d", i);
		HANDLE hMutex = OpenMutex(
				MUTEX_ALL_ACCESS,				// request full access
				FALSE,							// handle not inheritable
				text );							// object name

		if (hMutex == NULL)
		{
			g_mutex = CreateMutex(
				NULL,							// default security descriptor
				FALSE,							// mutex not owned
				text );							// object name
			ApConfig::SetInstance(i);
			break;
		}
		else
		{
			CloseHandle(hMutex);
		}
	}

	// Initialise GLFW
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	glfwGetDesktopMode(&dvm);

	project->Init();

	// Frame counter and window settings variables
	int frame = 0;
	int redBits = 8, greenBits = 8, blueBits = 8;
	int alphaBits = 8, depthBits = 24, stencilBits = 8;
	m_width = RsUtil::GetWidth();
	m_height = RsUtil::GetHeight();

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

	// Create a window
	if(!glfwOpenWindow(m_width, m_height, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, GLFW_WINDOW))
	{
		fprintf(stderr, "Failed to open GLFW window\n");

		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Set listeners
	glfwSetWindowCloseCallback(windowCloseListener);

	int width, height;
	GetDesktopResolution(width, height);
	glfwSetWindowPos((width / 2) - (RsUtil::GetWidth() / 2), (height / 2) - (RsUtil::GetHeight() / 2));

	// Cache the window handle
	int handle = glfwGetWindowParam( -1 );

	// Create the game mouse
	m_gameMouse.Setup( (HWND)handle );
	m_gameMouse.CreateOnDevice();

	// Create the render system
	RsImpl::pInstance()->Create();

	SdSoundWinGL::CreateManager();

	BtTimeDX11::Init();

	//glfwOpenWindowHint( GLFW_REFRESH_RATE, 1.0f / 30.0f );
	// Set the game window title
	BtChar title[32];
	strcpy(title, ApConfig::GetTitle());
	BtU32 instances = ApConfig::GetNumInstances();
	if (instances > 0)
	{
		BtChar instanceNum[32];
		sprintf(instanceNum, "_%d", instances);
		strcat(title, instanceNum);
	}
	glfwSetWindowTitle(title);

	{ // Read in the window position
		int l, t;
		FsFile windowPosIn;
		windowPosIn.Open(title, FsMode_Read);
		if (windowPosIn.IsOpen())
		{
			windowPosIn.Read(l);
			windowPosIn.Read(t);
			windowPosIn.Close();
			glfwSetWindowPos(l, t);
		}

		DeleteFile(title);
	}

	// Ensure we can capture the escape key being pressed below
	glfwEnable(GLFW_STICKY_KEYS);

	// Enable vertical sync (on cards that support it)
	//glfwSwapInterval(1);

	// Hide the cursor
	glfwDisable(GLFW_MOUSE_CURSOR);

	project->Create();
	project->Reset();
	do
	{
		// Get window size (may be different than the requested size)
		glfwGetWindowSize(&m_width, &m_height);

		// Set the window size
		glViewport(0, 0, m_width, m_height);

		BtFloat dt = BtTime::GetTick();
		BtFloat elapsedTime = BtTime::GetElapsedTimeInSeconds();
		BtFloat deltaTime = elapsedTime - m_lastGameLogic;

		m_lastGameLogic = elapsedTime;

		// Add the excess
		m_frameRemainder += deltaTime;

		// Cap the frame
		m_frameRemainder = BtClamp(m_frameRemainder, (BtFloat)0, 1.0f);

		BtBool isExiting = BtFalse;

		while( m_frameRemainder >= dt )
		{
			// Update the keys
			input.Update();

			// Update the game logic
			GameUpdate();

			// Remove the dt from the frame remainder
			m_frameRemainder -= dt;
		}

		if((isClosing == BtFalse) && (project->IsClosing() == BtFalse))
		{
			// Render
			project->Render();

			// Render
			RsImpl::pInstance()->Render();
		}

		// Swap buffers
		glfwSwapBuffers();
	} // Check if the ESC key was pressed or the window was closed
	while(project->IsClosed() == BtFalse);

	{
		RECT r;
		GetWindowRect((HWND)handle, &r);
		int l = r.left;
		int t = r.top;
		FsFile file;
		file.Open(title, FsMode_WriteAscii);
		if (file.IsOpen())
		{
			file.Write(l);
			file.Write(t);
			file.Close();
		}
	}

	// Destroy the renderer
	RsImpl::pInstance()->Destroy();

	// Destroy the sound
	SdSoundWinGL::DestroyManager();

	// Remove the game mouse
	m_gameMouse.RemoveFromDevice();

	// Destroy the project
	project->Destroy();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	// Close the mutex
	CloseHandle(g_mutex);
}

////////////////////////////////////////////////////////////////////////////////
// GameUpdate

void GameUpdate()
{
	// Update the game mouse
	m_gameMouse.Update();

	// Cache the number of mice
	BtU32 numMice = m_gameMouse.GetNumMice();

	for( BtU32 device=0; device<numMice; device++ )
	{
		// Cache the position of the mouse pointer
		MtVector2 v2Position = m_gameMouse.GetPosition( device );

		BtFloat x = v2Position.x;
		BtFloat y = v2Position.y;

		BtU32 mouseIndex = device * 2;

		if( m_gameMouse.Pressed( 0, device ) == BtTrue )
		{
			ShTouch::BeginTouch( mouseIndex, MtVector2( x, y ) );
		}

		if( m_gameMouse.IsAlive( device ) == BtTrue )
		{
			ShTouch::SetAlive( mouseIndex );
		}

		ShTouch::MoveTouch( mouseIndex, MtVector2( x, y ) );

		if( m_gameMouse.Released( 0, device ) == BtTrue )
		{
			ShTouch::EndTouch( mouseIndex, MtVector2( x, y ) );
		}

		BtU32 button2 = mouseIndex + 1;

		if( m_gameMouse.Pressed( 1, device ) == BtTrue )
		{
			ShTouch::BeginTouch( button2, MtVector2( x, y ) );
		}

		ShTouch::MoveTouch( button2, MtVector2( x, y ) );

		if( m_gameMouse.Released( 1, device ) == BtTrue )
		{
			ShTouch::EndTouch( button2, MtVector2( x, y ) );
		}		
	}

	ShTouch::Update();
	ShJoystick::Update();
	ShKeyboard::Update();

	//if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_F4 ) )
	//{
	//	windowCloseListener();
	//	project->SetClosing();
	//}

	// Update
	if( project->IsClosed() == BtFalse )
	{
		project->Update();
	}
}
