#include <Windows.h>
#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#include "Game.h"
#include "ResManager.h"
#include "Model.h"
#include "Image.h"
#include "Camera.h"
#include "Color.h"
#include "Shader.h"
#include "Matrix4.h"
#include <SFML/Window.hpp>
#include <SFML/Window/ContextSettings.hpp>

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int iCmdShow)
{
	//cuInit(0);
	const int ScreenW = 1024;
	const int ScreenH = 720;
	CLogger::GetSingleton()->Enable(CONSOLE | TXT);
	sf::Window App;
	sf::ContextSettings Settings;
	Settings.DepthBits         = 24; // Request a 24 bits depth buffer
	//Settings.StencilBits       = 8;  // Request a 8 bits stencil buffer
	//Settings.AntialiasingLevel = 2;  // Request 2 levels of antialiasing
	Settings.MajorVersion = 3; // OpenGL 3.3
	Settings.MinorVersion = 3;
	App.Create(sf::VideoMode(ScreenW,ScreenH), "Sample_Simple", 7, Settings);
	glewInit();

	int Err = glGetError();

	//CResManager::GetSingleton()->AddCatalogEntry("normal_map", "normal.jpg");
	auto BallModel = CResManager::GetSingleton()->GetResource<CModel>("Cube");

	float FrameTimer = 0.f;
	const float UpdateTime = 1.f/30.f;
	sf::Clock Clock;
	
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glDisable (GL_BLEND);
	glDisable (GL_ALPHA_TEST);
	//glEnable (GL_CULL_FACE);
	glCullFace(GL_BACK);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);

	glClearColor(0.5f, 0.f, 0.f, 1.f);
	glColor4f (1.f, 1.f, 1.f, 1.f);

	CShader* Shader = CResManager::GetSingleton()->GetResource<CShader>("shader");
	CImage* Deski = CResManager::GetSingleton()->GetResource<CImage>("crate_tex");
	CImage* Trawa = CResManager::GetSingleton()->GetResource<CImage>("grass_tex");
	CImage* Skaly = CResManager::GetSingleton()->GetResource<CImage>("rock_tex");
	//CImage* NormMap = CResManager::GetSingleton()->GetResource<CImage>("normal_map");

	Shader->Bind();
	Shader->DebugDump();

	Shader->SetTex("Texture", 0);
	Shader->SetTex("Bumpmap", 1);
	Deski->Bind(0);
	Trawa->Bind(1);

	int VMatLoc = Shader->GetUniformLocation("ViewMatrix");
	int PMatLoc = Shader->GetUniformLocation("ProjectionMatrix");
	int LightPosLoc = Shader->GetUniformLocation("LightPosition");

	CMatrix4 PMatrix, SMatrix;
	CMatrix4 VMatrix;
	CVector3 LightPosition(500.f, -500.f, 500.f);
	CColor	LightAmbient(.0f, .0f, .0f),
			LightDiffuse(.6f, 0.6f, .6f),
			LightSpecular(.0f, .5f, 0.f);

	CColor	MatAmbient(.2f, .2f, .2f),
			MatDiffuse(1.0f, 1.0f, 1.0f),
			MatSpecular(.9f, .9f, 0.9f);

	PMatrix = CCamera::CreateProjection(45, 16.f/10.f, 0.1f, 1000.f);
	Shader->SetUniformMatrix4("ProjectionMatrix", PMatrix);

	SMatrix = CMatrix4::CreateIdentity();
	SMatrix = CCamera::CreateScale(2.0, 2.0, 2.0);

	Shader->SetUniformVector3("LightPosition", LightPosition);
	Shader->SetUniformColor("Light0.Ambient", LightAmbient);
	Shader->SetUniformColor("Light0.Diffuse", LightDiffuse);
	Shader->SetUniformColor("Light0.Specular", LightSpecular);

	Shader->SetUniformColor("FrontMaterial.Ambient", MatAmbient);
	Shader->SetUniformColor("FrontMaterial.Diffuse", MatDiffuse);
	Shader->SetUniformColor("FrontMaterial.Specular", MatSpecular);

	while (App.IsOpened())
	{
		// Process events
		sf::Event Event;
		while (App.PollEvent(Event))
		{
			switch (Event.Type)
			{
			case sf::Event::MouseWheelMoved:
				//Game.MouseWheel(Event.MouseWheel.Delta);
				break;

			case sf::Event::MouseMoved:
				//CGUI::GetSingleton()->MouseMove (Event);
				//Game.MouseMove(Event);
				break;

			// Close window : exit
			case sf::Event::Closed:
				App.Close();
				break;
			case sf::Event::KeyPressed:
				if (Event.Key.Code == sf::Key::Escape)
					App.Close();
				//CGUI::GetSingleton()->KeyPress (Event);
				break;
			case sf::Event::TextEntered:
				//CGUI::GetSingleton()->TextEnter (Event);
				break;
			case sf::Event::MouseButtonReleased:
				//CGUI::GetSingleton()->MouseRelease (Event);
				break;
			case sf::Event::MouseButtonPressed:
				//CGUI::GetSingleton()->MousePress (Event);
				break;
			}
		}

		FrameTimer += Clock.GetElapsedTime();
		Clock.Reset();
		while (FrameTimer > UpdateTime)
		{
			FrameTimer -= UpdateTime;
			//Game.Steer(App.GetInput());
			//Game.Update ();
		}
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float MouseX, MouseY;
		const sf::Input& Input = App.GetInput();
		MouseX = ((float)(Input.GetMouseX()) / (float)(App.GetWidth()) - 0.5f) * 4.f;
		MouseY = ((float)(Input.GetMouseY()) / (float)(App.GetHeight()) - 0.5f) * 4.f;

		CVector3 Eye (MouseX, MouseY, 5);
		CVector3 Center (0.5, 0.5, 0.5);
		VMatrix = CCamera::CreateModelview(Eye, Center);
		VMatrix.Mult(SMatrix);
		glUniformMatrix4fv(VMatLoc, 1, GL_FALSE, VMatrix.GetRawData());
		
		BallModel->Draw();

		App.Display();
		sf::Sleep(0.001f);
	}
	// Wersja ze skryptami : u¿ycie CGameObject i tworzenie w³asnych typów w Lua
	// Wersja bez skryptów : dziedziczenie w³asnych typów po CGameObject

	return 0;
}