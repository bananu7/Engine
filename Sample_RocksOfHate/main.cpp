#include <Windows.h>
#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#include "Game.h"
#include "ResManager.h"
#include "Model.h"
#include "Camera.h"
#include <SFML/Window.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include "Ship.h"
#include "Bullet.h"
#include "EntityMgr.h"
#include "Rock.h"
#include "Shader.h"
#include <GL/glew.h>

// Rocks of Hate

void InitializeGL();

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int iCmdShow)
{
	CLogger::GetSingleton()->Enable(CONSOLE | TXT);
	sf::Window App;
	{
		sf::ContextSettings Settings;
		Settings.depthBits         = 24; // Request a 24 bits depth buffer
		//Settings.StencilBits       = 8;  // Request a 8 bits stencil buffer
		//Settings.AntialiasingLevel = 2;  // Request 2 levels of antialiasing
		Settings.majorVersion = 3; // OpenGL 3.2
		Settings.minorVersion = 2;															 
		App.create(sf::VideoMode(1280,800), "Sample_Simple", 7, Settings);

		CResManager::GetSingleton()->AddCatalogEntry("Ball", SLoadParams("Ball.obj"));
		CResManager::GetSingleton()->AddCatalogEntry("Ship", SLoadParams("ship.obj"));
		CResManager::GetSingleton()->AddCatalogEntry("Cube", SLoadParams("Cube.obj"));

		CResManager::GetSingleton()->AddCatalogEntry("texturing", SLoadParams("texturing"));
		glewInit();
	}

	InitializeGL();
	//CResManager::GetSingleton()->GetResource<CShader>("texturing")->Bind();

	float FrameTimer = 0.f;
	const float UpdateTime = 1.f/30.f;
	sf::Clock Clock;

	CShip* Player = new CShip;
	CEntityMgr Emgr;
	Emgr.AddObject(Player);

	for (int i = 0; i < 20; ++i)
		Emgr.AddObject(new CRock());

	while (App.isOpen())
	{
		// Process events
		sf::Event Event;
		while (App.pollEvent(Event))
		{
			switch (Event.type)
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
				App.close();
				break;
			case sf::Event::KeyPressed:
				switch (Event.key.code)
				{
				case sf::Keyboard::Escape:
					App.close();
					break;
				case sf::Keyboard::Space:
					CBullet* Temp = new CBullet(Player);
					Emgr.AddObject(Temp);
					break;
				}
				
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

		FrameTimer += Clock.getElapsedTime().asMilliseconds();
		Clock.restart();
		while (FrameTimer > UpdateTime)
		{
			FrameTimer -= UpdateTime;
			Emgr.Update();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				Player->Acc();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				Player->Turn(false);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				Player->Turn(true);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				Player->Dec();

			//Game.Steer(App.GetInput());
			//Game.Update ();
		}
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable (GL_TEXTURE_2D);
		//CResManager::GetSingleton()->GetResource<CShader>("shader")->Bind();
		//Game.Draw();
		//CShader::DisableAll();

		//App.SaveGLStates();
		//CGUI::GetSingleton()->Draw();
//		CGUI::GetSingleton()->DrawDebug();
		//App.RestoreGLStates();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(45, 1280./800., 0.01, 1000);
		glOrtho (-24, 24, -15, 15, -100, 100);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		/*CCamera::SphericalProjection(
			CVector3(0.f,0.f,0.f),
			0.f,
			CVector2<float>(0.5f, .5f),
			100.f);*/
		glTranslatef (0.f,0.f,+10.f);

		/*float MouseX, MouseY;
		const sf::Input& Input = App.GetInput();
		MouseX = ((float)(Input.GetMouseX()) / (float)(App.GetWidth()) - 0.5f) * 8.f;
		MouseY = ((float)(Input.GetMouseY()) / (float)(App.GetHeight()) - 0.5f) * 8.f;

		/*glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho (-10, 10, -10, 10, -100, 100);*

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		CCamera::SphericalProjection(
			CVector3(0.f, 0.f, 0.f),
			0.f,
			CVector2<float>(MouseX, MouseY),
			5.f
			);*/
		
		/*glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glVertex2f (-5, -5);
		glVertex2f (5, -5);
		glVertex2f (5, 5);
		glVertex2f (-5, 5);
		glEnd();*/

		//BallModel->Draw();
		Emgr.Draw();

		App.display();
		//sf::sleep(sf::Time::Time(0));
	}
	// Wersja ze skryptami : u¿ycie CGameObject i tworzenie w³asnych typów w Lua
	// Wersja bez skryptów : dziedziczenie w³asnych typów po CGameObject

	/*CGame::GetSingleton()->Init(lpCmdLine);
	CGame::GetSingleton()->Go();
	CGame::GetSingleton()->Destroy();*/
	return 0;
}

void InitializeGL()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_TEXTURE_2D);
	glEnable (GL_LIGHTING);
	glDisable (GL_BLEND);
	glDisable (GL_ALPHA_TEST);
	//glEnable (GL_CULL_FACE);
	glCullFace(GL_BACK);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.f, 0.2f, 1.f);
	glColor4f (1.f, 1.f, 1.f, 1.f);

	GLfloat light_ambient[] =	{ .2f, .2f, .2f, 1.0f };
	GLfloat light_diffuse[] =	{ .8f, .8f, .8f, 1.0f };
	GLfloat light_specular[] =	{ .0f, .0f, .0f, 1.0f };
	GLfloat light_position[] =	{ -50.f, 50.f, 50.f, 0.f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);

}