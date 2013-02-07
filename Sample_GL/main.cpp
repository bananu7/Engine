#include "WinAPIOpenGLWindow.h"

// ENGINE RELATED
#include "Shader.h"
#include "VertexAttributeArray.h"
#include "VertexBuffer.h"
#include "Camera.h"

#include <boost/noncopyable.hpp>

#include <array>
#include <vector>
#include <string>
#include <exception>
#include <map>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>


bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = true;		// Window Active Flag Set To TRUE By Default
bool	fullscreen= true;	// Fullscreen Flag Set To Fullscreen Mode By Default

const unsigned int ScreenXSize = 1024;
const unsigned int ScreenYSize = 768;

CWinAPIOpenGLWindow Win;

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		/*case WM_SIZE:								// Resize The OpenGL Window
		{
			Re(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}*/
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int InitGL(void)										// All Setup For OpenGL Goes Here
{
	/*glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Test To Do
	//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDisable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
*/

	return TRUE;										// Initialization Went OK
}

void CheckForGLError()
{
	int Err = glGetError();
	if (Err != 0)
		_CrtDbgBreak();
}

class CSampleRenderer : boost::noncopyable
{
	std::array<CVertexAttributeArray, 2> VAOs;
	std::array<CVertexBuffer, 3> VBOs;

public:
	void InitData(int in_Position, int in_Color)
	{
			// First VAO setup
		VAOs[0].Bind();
		{
			const float vert[] = { 
			-0.3,	0.5,	-1.0,
			-0.8,	-0.5,	-1.0,
			0.2,	-0.5,	-1.0
			};
			const float colors[] = {
				1.0,	0.0,	0.0,
				0.0,	1.0,	0.0,
				0.0,	0.0,	1.0
			};

			VBOs[0].Bind();
			VBOs[0].LoadData(vert, sizeof(vert));
			glVertexAttribPointer(in_Position, 3, GL_FLOAT, GL_FALSE, 0, 0); 
			VAOs[0].EnableAttributeArray(0);
 
			VBOs[1].Bind();
			VBOs[1].LoadData(colors, sizeof(colors));
			glVertexAttribPointer(in_Color, 3, GL_FLOAT, GL_FALSE, 0, 0);
			VAOs[0].EnableAttributeArray(1);
		}

		// Second VAO setup	
		VAOs[1].Bind();
		{
			const float vert[] = {
				-0.2,	0.5,	-1.0,
				0.3,	-0.5,	-1.0,
				0.8,	0.5,	-1.0
			};

			VBOs[2].Bind();
			VBOs[2].LoadData(vert, sizeof(vert));
			glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
			VAOs[1].EnableAttributeArray(0);
			glVertexAttrib3f((GLuint)1, 0.0, 0.0, 1.0); // set constant color attribute
 
			glBindVertexArray(0);
		}
		CheckForGLError();
	}
	void Render()
	{
		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		VAOs[0].Bind();
		VAOs[0].Draw(0, 3);

		VAOs[1].Bind();
		VAOs[1].Draw(0, 3);
	}
	void Benchmark ()
	{
		int MaxTextureImageUnits; // maksymalna ilosc samplerow
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureImageUnits);
	}


	CSampleRenderer() { }
};


int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int iCmdShow)
{
	Win.Create("OpenGLUI test", ScreenXSize, ScreenYSize, 32, false, WndProc);
	Win.Resize(ScreenXSize, ScreenYSize);

	CSampleRenderer R;

	R.Benchmark();
	//InitGL();

	CShader Shader;
	std::map<std::string, std::string> Data;
	Data["vert"] = "simple.vert";
	Data["frag"] = "simple.frag";
	CSimpleFileLoader ShaderPaths(Data);
	
	std::string Result = Shader.Load(ShaderPaths);
	if (Result != "") 
	{
		{
			std::ofstream Log ("log.txt");
			Log << Result;
		}
		throw std::exception(Result.c_str());
	}
	Shader.Bind();
	Shader.SetUniformMatrix4("ViewMatrix", glm::translate(glm::mat4(), glm::vec3(0, 0, -10)));
	Shader.SetUniformMatrix4("ModelMatrix", glm::mat4());
	Shader.SetUniformMatrix4("ProjectionMatrix", CCamera::CreateOrtho(-3, -3, 3, 3, -100, 100));

	R.InitData(Shader.GetAttribLocation("in_Position"), Shader.GetAttribLocation("in_Color"));

	bool Run = true;
	do 
	{
		MSG	msg;
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				Run = false;							
			}
			else if (msg.message==WM_KEYDOWN)
			{
			}
			else if (msg.message==WM_MOUSEMOVE)
			{
			}
			else if (msg.message==WM_LBUTTONDOWN)
			{
			}
			else if (msg.message==WM_LBUTTONUP)
			{
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		// rendering
		R.Render();
		Win.Display();
	} while (Run);

	return 0;
}

//=========================================================================================

