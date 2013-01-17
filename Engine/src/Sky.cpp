
#include "Sky.h"
#include "ResManager.h"
#include "Image.h"
#include "Shader.h"

void CSky::Draw ()
{
	const float Size = 400.f;
	float Xoff, Yoff;
	const float TSize = 1.f;
	glEnable(GL_TEXTURE_2D);
	CShader::DisableAll();
	glDisable(GL_LIGHTING);
	CResManager::GetSingleton()->GetResource<CShader>("texture_shader")->Bind();
	CResManager::GetSingleton()->GetResource<CShader>("texture_shader")->SetTex("tex_img", 0);
	glActiveTexture(GL_TEXTURE0);
	CResManager::GetSingleton()->GetResource<CImage>("skybox")->Bind();
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.f);
		glVertex3f(Size, -Size, -Size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(Size, -Size, +Size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(Size, Size, +Size);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(Size, Size, -Size);

		glTexCoord2f(1.0f, 1.f);
		glVertex3f(-Size, -Size, -Size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-Size, -Size, +Size);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-Size, Size, +Size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-Size, Size, -Size);

		glTexCoord2f(1.0f, 1.f);
		glVertex3f(-Size, -Size, -Size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-Size, Size, -Size);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(Size, Size, -Size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(Size, -Size, -Size);

		glTexCoord2f(1.0f, 1.f);
		glVertex3f(Size, -Size, +Size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(Size, Size, +Size);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-Size, Size, +Size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-Size, -Size, +Size);
	glEnd();
	glEnable(GL_LIGHTING);
}

CSky::CSky(void)
{
}


CSky::~CSky(void)
{
}
