
#include "ResManager.h"
#include "Model.h"
#include "CameraAdds.h"
#include "Scene.h"
#include "Ground.h"
#include "Skybox.h"
#include "Player.h"
#include "Misc.h"
#include "Shader.h"
#include "ShaderSet.h"
#include "CameraAdds.h"
#include "Color.h"
#include "Image.h"
#include "Mirror.h"

#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>

#include <SFML/Window.hpp>
#include <SFML/Window/ContextSettings.hpp>

#include "Physics.h"
#include "GUI.h"
#include "Graph.h"
#include "Font.h"

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int iCmdShow)
{
	const int ScreenW = 1024;
	const int ScreenH = 720;
	CLogger::GetSingleton()->Enable(CONSOLE | TXT);
	sf::Window App;
	sf::ContextSettings Settings;
	Settings.DepthBits         = 24; // Request a 24 bits depth buffer
	//Settings.StencilBits       = 8;  // Request a 8 bits stencil buffer
	//Settings.AntialiasingLevel = 2;  // Request 2 levels of antialiasing
	Settings.MajorVersion = 3; // OpenGL 3.2
	Settings.MinorVersion = 2;
	App.Create(sf::VideoMode(ScreenW,ScreenH), "Sample_Schmetterling", 7, Settings);
	glewInit();

	CResManager::GetSingleton()->LoadCatalog("../Resources/Resources_sample_Butterfly.xml");

	CCameraTrack		Camera;
	CCameraFly			CameraFly;
	CScene				Scene;

	CGround* Ground = CResManager::GetSingleton()->GetResource<CGround>("ground");
	CSkybox* Skybox = CSkybox::GetSingleton()->Init(
		Ground->GetWScale() * Ground->GetSize().X,
		Ground->GetHScale()
		);

	#pragma region Fizyka
	CPhysics* Physics = CPhysics::GetSingleton();
	Physics->Init();
	Physics->CreateGround(Ground->GetSize().X, Ground->GetSize().Y, Ground->GetHScale(), Ground->GetWScale(), Ground->GetRawData());
	Physics->LoadTriangleMesh("../Resources/simple_cube.fbx", CVector3(100.f, 135.f, 100.f));
	#pragma endregion

	CPlayer				Player;

	Scene.m_GroundSampler  = [&](float x, float y) -> float { return Ground->GetHeight(CVector2<float>(x,y)); };
	Scene.LoadFromFile("../Resources/scene.xml");

	float FrameTimer = 0.f;
	const float UpdateTime = 1.f/60.f;

	bool BindMouse = true;
	sf::Clock Clock;

	CShaderSet ShaderSet;
	CShader* Light = CResManager::GetSingleton()->GetResource<CShader>("shader_light");
	CShader* Shader = CResManager::GetSingleton()->GetResource<CShader>("shader_heightmap");
	CShader* Simple = CResManager::GetSingleton()->GetResource<CShader>("shader_simple");
	CShader* Texturing = CResManager::GetSingleton()->GetResource<CShader>("shader_texturing");
	ShaderSet.AddShader(Light);
	ShaderSet.AddShader(Shader);
	ShaderSet.AddShader(Simple);
	ShaderSet.AddShader(Texturing);
	ShaderSet.AddShader(CResManager::GetSingleton()->GetResource<CShader>("shader_mirror"));

	#pragma region Initialization
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_TEXTURE_2D);
	glDisable (GL_LIGHTING);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable (GL_ALPHA_TEST);
	//glEnable (GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 0.f, 0.f, 1.f);
	glColor4f (1.f, 1.f, 1.f, 1.f);
	glClearDepth(100.0f);

	CVector3 LightPosition(500.f, 500.f, 500.f);
	CColor	LightAmbient (.2f, .2f, .2f),
			LightDiffuse (.6f, .6f, .6f),
			LightSpecular(.5f, .5f, .5f);

	CColor	MatAmbient (.2f, .2f, .2f),
			MatDiffuse (1.f, 1.f, 1.f),
			MatSpecular(.9f, .9f, .9f);
	

	Shader->Bind();	

	//Camera.Position = CVector3(150.f, 100.f, 64.f);

	Light->BindAttribLocation("in_Position", 1);
	Light->BindAttribLocation("in_Position", 0);
	Light->Compile();
	
	Shader->BindAttribLocation("in_Position", 0);
	Shader->BindAttribLocation("in_Normal", 1);
	Shader->BindAttribLocation("in_TexCoord", 2);
	Shader->Compile();

	ShaderSet.SetUniformVector3("LightPosition", LightPosition);
	ShaderSet.SetUniformColor("Light0.Ambient", LightAmbient);
	ShaderSet.SetUniformColor("Light0.Diffuse", LightDiffuse);
	ShaderSet.SetUniformColor("Light0.Specular", LightSpecular);

	ShaderSet.SetUniformColor("FrontMaterial.Ambient", MatAmbient);
	ShaderSet.SetUniformColor("FrontMaterial.Diffuse", MatDiffuse);
	ShaderSet.SetUniformColor("FrontMaterial.Specular", MatSpecular);
	bool Valid = Light->Validate();

	// Kamera
	Camera.SetProjectionMat(CCamera::CreateProjection(45, 16.f/10.f, 0.01f, 2000.f));
	Camera.CalculateProjection();
	CameraFly.SetProjectionMat(CCamera::CreateProjection(45, 16.f/10.f, 0.01f, 2000.f));
	CameraFly.CalculateProjection();
	const CMatrix4& PMatrix = Camera.GetProjectionMat();

	// Ground Shader? - Begin

	std::vector<CImage*> GroundTextures;

	GroundTextures.push_back(CResManager::GetSingleton()->GetResource<CImage>("grass_tex"));
	GroundTextures.push_back(CResManager::GetSingleton()->GetResource<CImage>("rock_tex"));

	for (int it= 0,itE= GroundTextures.size(); it<itE; ++it)
		GroundTextures[it]->Bind(it);

	for (int it= 0,itE= GroundTextures.size(); it<itE; ++it)
		ShaderSet.SetTex("Texture"+boost::lexical_cast<string>(it),it);

	ShaderSet.SetTex("Bumpmap",GroundTextures.size());
	
	ShaderSet.SetUniformMatrix4("ProjectionMatrix", PMatrix);

	CameraFly.Position = CVector3(50.f, 150.f, 10.f);
	Camera.Target = CVector3(10.f, 10.f, 10.f);
	Camera.Direction = 0.f;
	Camera.Radius = 10.f;

	// ustawianie stalych grafu
	const float Border		= 20.f;						// odl od krawedzi ekranu
	const float GraphW		= 500.f;					// Wymiar szerokosci grafu
	const float GraphH		= 200.f;					// wymiar wysokosci grafu
	const float GraphX		= Border;					// wspolrzedna X gornego lewego wierzcholka
	const float GraphY		= ScreenH-Border-GraphH;	// wspolrzedna Y gornego lewego wierzcholka
	const float LineSize	= 2.f;						// grubosc ramki
	const float GraphScale	= 1.f;						// wspolczynnik przeskalowania grafu po Y

	const float LEDSize		= 10.f;
	const float LEDBoxSize	= 200.f;
	const float LEDBoxX		= ScreenW-Border-LEDBoxSize;
	const float LEDBoxY		= ScreenH-Border-LEDBoxSize;
	const CColor LEDColor	= CColor(.7f,1.f,.2f);
	
	CGUI::GetSingleton()->Inititalise(ScreenW, ScreenH);

	auto BoxPtr1 = CGUI::GetSingleton()->AddElement(CBox(GraphX-LineSize,GraphY-LineSize,GraphW+2*LineSize,GraphH+2*LineSize,CColor(1.f,1.f,1.f)));
	auto BoxPtr0 = CGUI::GetSingleton()->AddElement(CBox(GraphX,GraphY,GraphW,GraphH,CColor(.1f,.1f,.1f)));
	//auto Graph0Ptr  = CGUI::GetSingleton()->AddElement(CGraph(GraphX,GraphY,GraphW,GraphH,CColor(.3f,1.f,0.f),GraphScale,true));
	//auto Graph1Ptr  = CGUI::GetSingleton()->AddElement(CGraph(GraphX,GraphY,GraphW,GraphH,CColor(.5f,.2f,.7f),GraphScale,false));
	auto ConsolePtr = CGUI::GetSingleton()->AddElement(CFont(GraphX+LineSize,GraphY+LineSize,"texturefont",CColor(1.f,.3f,.0f),CVector2<float>(16.f,16.f)));
	//ConsolePtr->Set("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
	//ConsolePtr->Push("abcdefghijklmnoqprstuvwxyz\n");
	//ConsolePtr->Push("1234567890.,\n");
	/*
	// Ustawianie Cerebro
		// tlo
	auto BoxPtr4 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX-LineSize,LEDBoxY-LineSize,LEDBoxSize+2*LineSize,LEDBoxSize+2*LineSize,CColor(1.f,1.f,1.f)));
	auto BoxPtr3 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX,LEDBoxY,LEDBoxSize,LEDBoxSize,CColor(.1f,.1f,.1f)));
		// diody
	auto LEDPtr00 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*3,LEDBoxY+LEDBoxSize/9*1,LEDSize,LEDSize,LEDColor));
	auto LEDPtr01 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*4,LEDBoxY+LEDBoxSize/9*1,LEDSize,LEDSize,LEDColor));

	auto LEDPtr02 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*2,LEDBoxY+LEDBoxSize/9*2,LEDSize,LEDSize,LEDColor));
	auto LEDPtr03 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*5,LEDBoxY+LEDBoxSize/9*2,LEDSize,LEDSize,LEDColor));

	auto LEDPtr04 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*3,LEDBoxY+LEDBoxSize/9*3,LEDSize,LEDSize,LEDColor));
	auto LEDPtr05 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*4,LEDBoxY+LEDBoxSize/9*3,LEDSize,LEDSize,LEDColor));

	auto LEDPtr06 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*2,LEDBoxY+LEDBoxSize/9*4,LEDSize,LEDSize,LEDColor));
	auto LEDPtr07 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*5,LEDBoxY+LEDBoxSize/9*4,LEDSize,LEDSize,LEDColor));

	auto LEDPtr08 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*1,LEDBoxY+LEDBoxSize/9*5,LEDSize,LEDSize,LEDColor));
	auto LEDPtr09 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*6,LEDBoxY+LEDBoxSize/9*5,LEDSize,LEDSize,LEDColor));

	auto LEDPtr10 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*2,LEDBoxY+LEDBoxSize/9*6,LEDSize,LEDSize,LEDColor));
	auto LEDPtr11 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*5,LEDBoxY+LEDBoxSize/9*6,LEDSize,LEDSize,LEDColor));

	auto LEDPtr12 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*2,LEDBoxY+LEDBoxSize/9*7,LEDSize,LEDSize,LEDColor));
	auto LEDPtr13 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*5,LEDBoxY+LEDBoxSize/9*7,LEDSize,LEDSize,LEDColor));

	auto LEDPtr14 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*3,LEDBoxY+LEDBoxSize/9*8,LEDSize,LEDSize,LEDColor));
	auto LEDPtr15 = CGUI::GetSingleton()->AddElement(CBox(LEDBoxX+LEDBoxSize/7*4,LEDBoxY+LEDBoxSize/9*8,LEDSize,LEDSize,LEDColor));
	*/
	//for graph testing
	//float test  = 2.5f;
	//float otest = 1.f;
	#pragma endregion

	float m_Mood= 1.f;		// values from 0 to 1
	float m_MoodBool= 1.f;
	float m_MoodChangeRate= .001f;
	CMirror Mirror(CResManager::GetSingleton()->GetResource<CShader>("shader_mirror"));
	Mirror.Init();

	while (App.IsOpened())
	{
		#pragma region Process events
		sf::Event Event;
		while (App.PollEvent(Event))
		{
			switch (Event.Type)
			{
			case sf::Event::MouseWheelMoved:
				//Game.MouseWheel(Event.MouseWheel.Delta);
				break;

			case sf::Event::Closed:
				App.Close();
				break;
			case sf::Event::KeyPressed:
				if (Event.Key.Code == sf::Key::Escape)
					App.Close();
				break;
			case sf::Event::LostFocus:
				BindMouse = false;
				break;

			case sf::Event::GainedFocus:
				BindMouse = true;
				break;
			}
		}

		FrameTimer += Clock.GetElapsedTime();
		Clock.Reset();
		while (FrameTimer > UpdateTime)
		{
			FrameTimer -= UpdateTime;

			const sf::Input& Input = App.GetInput();
			const float CameraFlySpeed = 0.5f;
			if (Input.IsKeyDown(sf::Key::W))
				CameraFly.Fly(CameraFlySpeed);
			if (Input.IsKeyDown(sf::Key::S))
				CameraFly.Fly(-CameraFlySpeed);
			if (Input.IsKeyDown(sf::Key::A))
				CameraFly.Strafe(CameraFlySpeed);
			if (Input.IsKeyDown(sf::Key::D))
				CameraFly.Strafe(-CameraFlySpeed);

			if (Input.IsKeyDown(sf::Key::Up))
				Player.Acc();
			if (Input.IsKeyDown(sf::Key::Down))
				Player.Dec();
			if (Input.IsKeyDown(sf::Key::Left))
				Player.Turn(true);
			if (Input.IsKeyDown(sf::Key::Right))
				Player.Turn(false);

			Player.Update();
			Physics->SimulateStep();
		}
		#pragma endregion
	
		#pragma region Kamera
		// Dane
		float MouseX, MouseY;
		const sf::Input& Input = App.GetInput();
		MouseX = (float)Input.GetMouseX() / App.GetWidth();
		MouseY = (float)Input.GetMouseY() / App.GetHeight();
		MouseX -= 0.5f;
		MouseY -= 0.5f;
		if (BindMouse)
			App.SetCursorPosition(App.GetWidth()/2, App.GetHeight()/2);

		// Aktualizacje
		CameraFly.LookDir.X += MouseX;
		CameraFly.LookDir.Y += MouseY;
		CameraFly.CalculateView();

		Camera.Radius = 60.f;
		Camera.Target = Player.GetPosition();
		Camera.Direction = Player.GetRotation().Y;
		Camera.CalculateView();

		CMatrix4 VMatrix;

		// Wykomentuj niechciana kamere, chciana kamere odkomentuj
		VMatrix = CameraFly.GetViewMat();			// Kamera Latania
		//VMatrix = Camera.GetViewMat();			// Kamera Zza Motyla

		#pragma endregion
		
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ShaderSet.SetUniform1f("Mood", m_Mood);
		ShaderSet.SetUniformMatrix4("ProjectionMatrix", PMatrix);
		ShaderSet.SetUniformMatrix4("ModelMatrix", CMatrix4::CreateIdentity());
		ShaderSet.SetUniformMatrix4("ViewMatrix", VMatrix);

		// FIRST PASS
		ShaderSet.SetUniformMatrix4("ProjectionMatrix", PMatrix);
		Shader->Bind();
		for (int it= 0,itE= GroundTextures.size(); it<itE; ++it)
			GroundTextures[it]->Bind(it);
		if (App.IsOpened())
			Ground->Draw();
		Skybox->Draw();
		Scene.Draw();

		CResManager::GetSingleton()->GetResource<CImage>("texture_motyl")->Bind(0);
		CResManager::GetSingleton()->GetResource<CImage>("texture_motyl")->Bind(1);
		Player.Draw();
		CShader::DisableAll();

		// SECOND PASS
		
		CVector3 OldPos = CameraFly.Position;
		CVector2<float> OldLookDir = CameraFly.LookDir;
		/*
		Mirror.Bind();
		
			// Mirror View Matrix
			// Translate by mirror's position and size
			//CameraFly.Position.X += 50.f + 100.f;
			//CameraFly.Position.Y += 150.f;

			/*CameraFly.Position.Z = -CameraFly.Position.Z;
			CameraFly.LookDir.X = M_PI - CameraFly.LookDir.X;
			CameraFly.CalculateView();
			VMatrix = CameraFly.GetViewMat();
			// LookDir stays the same in current up vector and z mirror plane*/
		/*
			const float MirrorSize = 50.f;
			const float MirrorPosY = 150.f;
			const float MirrorPosX = 0.f;

			VMatrix = CCamera::CreateModelview(
				CVector3(-CameraFly.Position.X, CameraFly.Position.Y, CameraFly.Position.Z),
				CVector3(MirrorPosX + MirrorSize*0.5f, MirrorPosY + MirrorSize*0.5f, 0.f)
				);

			// Mirror Projection Matrix;
			float right = MirrorPosX - CameraFly.Position.X;
			float left = MirrorPosX + MirrorSize - CameraFly.Position.X;
			float bottom = MirrorPosY - CameraFly.Position.Y;
			float top = MirrorPosY + MirrorSize - CameraFly.Position.Y;

			float znear = CVector3(MirrorPosX+MirrorSize*0.5f, MirrorPosY+MirrorSize*0.5f, 0.f).Lenght(CameraFly.Position);

			CMatrix4 NewPMatrix = CCamera::CreateFrustum(
				left,
				right,			
				bottom,
				top,
				znear,
				1000.f);

			float TheAnswerToLifeUniverseAndEverything = atan2f(MirrorPosX+MirrorSize*0.5f-CameraFly.Position.X, CameraFly.Position.Z);
			float XRotFactor = atan2f(MirrorPosY+MirrorSize*0.5f-CameraFly.Position.Y, CameraFly.Position.Z);

			CMatrix4 ProjView = CCamera::CreateRotation(XRotFactor, TheAnswerToLifeUniverseAndEverything, 0.f);
			NewPMatrix.Mult(ProjView);

			ShaderSet.SetUniformMatrix4("ViewMatrix", VMatrix);
			ShaderSet.SetUniformMatrix4("ProjectionMatrix", NewPMatrix);

			Shader->Bind();
			for (int it= 0,itE= GroundTextures.size(); it<itE; ++it)
				GroundTextures[it]->Bind(it);
			if (App.IsOpened())
				Ground->Draw();
			Scene.Draw();
			//Skybox->Draw();

			CResManager::GetSingleton()->GetResource<CImage>("texture_motyl")->Bind(0);
			Player.Draw();
		Mirror.Unbind();*/
	
		CameraFly.Position = OldPos;
		CameraFly.LookDir = OldLookDir;
		CameraFly.CalculateView();
		VMatrix = CameraFly.GetViewMat();
		ShaderSet.SetUniformMatrix4("ViewMatrix", VMatrix);
		ShaderSet.SetUniformMatrix4("ProjectionMatrix", PMatrix);
		//Mirror.Draw();

		// GUI
		CShader::DisableAll();
		ConsolePtr->Set("Player's Mood == ");
		ConsolePtr->Push(boost::lexical_cast<std::string>(m_Mood));
		CGUI::GetSingleton()->Draw();
		//Sample Graph Usages
		
		// miganie boxa
		/*
		BoxPtr0->Resize(BoxPtr0->GetWidth()+otest,BoxPtr0->GetHeight()+otest);
		CGUI::GetSingleton()->GetElement(BoxPtr0->GetID())
			->Move(BoxPtr0->GetPositionX()-otest/2.f,BoxPtr0->GetPositionY()-otest/2.f);
			*/
		
		// dodawanie danych do wykresow
		/*
		Graph0Ptr->AddData(Player.GetPosition().Y);
		Graph1Ptr->AddData(20.f+10.f*test);

		CColor ColorTemp;

		#pragma region Przyklad sterowania kolorami Cerebro
		ColorTemp =		LEDPtr04->GetColor();
		ColorTemp.R=	.5f+otest/10.f;
		ColorTemp.G=	otest/5.f;
		ColorTemp.B=	0.f;
		LEDPtr04->SetColor(ColorTemp);
		
		ColorTemp =		LEDPtr07->GetColor();
		ColorTemp.B-=	otest/10.f;
		LEDPtr07->SetColor(ColorTemp);

		ColorTemp =		LEDPtr09->GetColor();
		ColorTemp.R+=	otest/10.f;
		LEDPtr09->SetColor(ColorTemp);

		ColorTemp =		LEDPtr14->GetColor();
		ColorTemp.R-=	otest/14.f;
		ColorTemp.G+=	otest/21.f;
		LEDPtr14->SetColor(ColorTemp);
		#pragma endregion
		
		LightPosition.X+=2.f;
		if (LightPosition.X >= 1000.f) LightPosition = CVector3(0.f, 500.f, 500.f);
		ShaderSet.SetUniformVector3("LightPosition", LightPosition);
		
		if		(test >= 5.f) otest =-.5f;
		else if (test <=-5.f) otest = .5f;
		//test+=otest;
		*/

		if		(m_Mood + m_MoodChangeRate * m_MoodBool > 1.f) m_MoodBool =-1;
		else if	(m_Mood + m_MoodChangeRate * m_MoodBool < 0.f) m_MoodBool = 1;
		m_Mood += m_MoodChangeRate * m_MoodBool;

		App.Display();
		sf::Sleep(0.001f);
	}

	return 0;
}