#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <list>

#include "Shader.h"
 
void keyboard(unsigned char key, int x, int y);
void display(void);
 
GLuint vboId;
GLuint vaoId;
GLuint programId;
GLuint shaderIds[4];
enum EShaderNames {
    VERTEX_SHADER_ID = 0,
    TESSELATION_SHADER_ID,
    GEOMETRY_SHADER_ID,
    FRAGMENT_SHADER_ID,
    SHADER_COUNT
};
 
//GL_, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER.
#define NL "\n"
 
using std::vector;
using std::string;
using std::unique_ptr;

template<class T> T& as_lvalue(T&& v){ return v; }
 
void CheckForError()
{
    GLint E = glGetError();
    if (E != 0)
        _CrtDbgBreak();
}
 
void init ()
{
    CheckForError();
 
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);
 
    glGenBuffers(1, &vboId);
    CheckForError();
 
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
 
    CheckForError();
 
    const float Verts[] = { 
        -0.5f, -0.5f, 0.f,
         0.5f, -0.5f, 0.f,
         0.5f,  0.5f, 0.f,
        -0.5f,  0.5f, 0.f,
    };
    glBufferData (GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
 
    CheckForError();
}
 

void initShaders()
{
	programId = glCreateProgram();
 
    shaderIds[VERTEX_SHADER_ID] = glCreateShader(GL_VERTEX_SHADER);
    shaderIds[TESSELATION_SHADER_ID] = glCreateShader(GL_TESS_EVALUATION_SHADER);
    shaderIds[GEOMETRY_SHADER_ID] = glCreateShader(GL_GEOMETRY_SHADER);
    shaderIds[FRAGMENT_SHADER_ID] = glCreateShader(GL_FRAGMENT_SHADER);

    CheckForError();
 
    std::array<std::string, SHADER_COUNT> ShaderSources;
    std::array<std::string, SHADER_COUNT> ShaderNames;
    ShaderNames[0] = "Vertex Shader";
    ShaderNames[1] = "Fragment Shader";
    ShaderNames[GEOMETRY_SHADER_ID] = "Geometry Shader";
 
    ShaderSources[VERTEX_SHADER_ID] =
            "#version 400 core"
          NL"precision highp float;"
          NL"layout(location = 0) in vec3 position;"
		  NL"out vec3 out_position;"
          NL"void main () {"
		  NL"    out_position = position;"
          NL"    gl_Position = vec4(position, 1.0);"
          NL"}";

    ShaderSources[FRAGMENT_SHADER_ID] =  
            "#version 400 core"
		  NL"in vec3 out_position;"
		  NL"uniform float bias;"
		  NL"out vec4 out_Color;"
          NL"void main () {"
        //NL"    out_Color = vec4(0.0, 1.0, 0.0, 1.0);"
		  NL"    out_Color = vec4(out_position.zxy + bias, 1.0);"
          NL"}";
 
    ShaderSources[GEOMETRY_SHADER_ID] =
        "#version 150"
      NL"layout(triangles) in;"
     // NL"layout(triangle_strip, max_vertices = 3) out;"
      NL"layout(line_strip, max_vertices = 4) out;"
 
      NL"void main() {"
      NL"    for(int i = 0; i < gl_in.length(); i++) {"
      NL"        gl_Position = gl_in[i].gl_Position;"
      NL"        EmitVertex();"
      NL"    }"
      NL"    gl_Position = gl_in[0].gl_Position;"
      NL"    EmitVertex();"
      NL"    EndPrimitive();"
      NL"}";
 
    std::vector<unsigned int> ActiveShaders;
    ActiveShaders.push_back(FRAGMENT_SHADER_ID);
    ActiveShaders.push_back(VERTEX_SHADER_ID);
    //ActiveShaders.push_back(GEOMETRY_SHADER_ID);
    for (unsigned i : ActiveShaders)
    {
        const char* Temp = ShaderSources[i].c_str();
        glShaderSource(shaderIds[i], 1, &Temp, nullptr);
        glCompileShader(shaderIds[i]);
        GLint compiled;
        glGetShaderiv(shaderIds[i], GL_COMPILE_STATUS, &compiled);
        if (compiled != GL_TRUE) {
            char Buffer[1000];
            glGetShaderInfoLog(shaderIds[i], sizeof(Buffer), &as_lvalue(int()), Buffer);
 
            std::cout << ShaderNames[i] << Buffer;
 
            _CrtDbgBreak();
        }
 
        glAttachShader(programId, shaderIds[i]);
    }

	glBindFragDataLocation(programId, 0, "out_Color");

    CheckForError();
 
    glLinkProgram(programId);
    GLint linked;
    glGetProgramiv(programId, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE)
    {
        char Buffer[1000];
        glGetProgramInfoLog(programId, 1000, &as_lvalue(int()), Buffer);
 
        std::cout << "Program link error : " << Buffer;
 
        _CrtDbgBreak();
    }
 
    glUseProgram(programId);
}

Program Shader;

class CRenderPass
{
	// input - vertices
	// vertex shader 1
	// fragment shader 1 -- we have to assume nontrivial
	// output stage - either screen or fbo

	/*
		shader.output += shadertarget::Screen
		shader.output += shadertarget::Fbo(myFbo);

		Using different output pretty much means:
		it's always one FBO, with multiple color attachments

			Buffer.attach(texture for attch0, text for attch1 ... etc)
			glDrawBuffers(COLOR_ATTCH_0, COLOR_ATTCH_1 ...etc)
	

		if the shader has fbo output, it has to be remembered which color attachments it's using
	*/

	std::list<Program> shaders;
};

void initShadersEngine()
{
	CSimpleDirectLoader::TDataMap Data;
	std::string Vert = 
            "#version 400 core"
          NL"precision highp float;"
          NL"layout(location = 0) in vec3 position;"
		  NL"out vec3 out_position;"
          NL"void main () {"
		  NL"    out_position = position;"
          NL"    gl_Position = vec4(position, 1.0);"
          NL"}";
	
	std::string Frag = 
            "#version 400 core"
		  NL"in vec3 out_position;"
		  NL"uniform float bias;"
		  NL"out vec4 out_Color;"
          NL"void main () {"
        //NL"    out_Color = vec4(0.0, 1.0, 0.0, 1.0);"
		  NL"    out_Color = vec4(out_position.zxy + bias, 1.0);"
          NL"}";

	using uc = unsigned char;

	Data["frag"] = vector<uc> (Frag.begin(), Frag.end());
	Data["vert"] = vector<uc> (Vert.begin(), Vert.end());
 
	auto Loader = CSimpleDirectLoader(Data);
	string Result = Shader.Load(Loader);
	if (!Result.empty())
		_CrtDbgBreak();
	Shader.Bind();
}

#ifdef _WINDOWS
int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
#else
int main(int argc, char** argv)
#endif

{
	#ifdef _WINDOWS
    int argc = 1;
    char* argv[] = {"program.exe"};
	#endif
    glutInit(&argc, argv);
    
    glutInitContextVersion (4,0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("GLUT Test");
 
    glewExperimental=TRUE;
    GLenum err=glewInit();
  if(err!=GLEW_OK)
    {
  	std::cout<<"glewInit failed, aborting.";
        _CrtDbgBreak();
    }
    // Explicit check for GLEW error
    err = glGetError();
    // If it's something different than INVALID_ENUM, we have a problem
    if (err != GL_INVALID_ENUM)
        _CrtDbgBreak();
 
    init();
	//initShaders();
	initShadersEngine();
 
    glutKeyboardFunc(&keyboard);
    glutDisplayFunc(&display);
 
    glutMainLoop();
 
    return EXIT_SUCCESS;
}
 
 
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '\x1B':
        exit(EXIT_SUCCESS);
        break;
    }
}
 
void display()
{
    glClearColor(0.5f, .5f, .5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
 
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
 
    glFlush();
}