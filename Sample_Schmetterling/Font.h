#pragma once
#include "GUI.h"
#include <Shader.h>
#include <Image.h>
#include <Color.h>
#include <Vector2.h>

using std::string;

class CFont :
    public CGUIElement
{
protected:
    CImage*                    m_Font;
    CVector2<float>            m_Scale;
    CColor                    m_Color;
    string                    m_Content;

public:
    void Draw();

    void SetColor(CColor Color);
    void Push(const string &String);
    void Set(const string &String);

    CFont(void);
    CFont(float X, float Y, const string &FontName, CColor Color, CVector2<float> FontScale);
    //~CFont(void);
};
