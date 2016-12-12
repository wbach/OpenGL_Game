#pragma once
#include <memory>
#include <vector>
#include <GL/glew.h>
#include "../Loader/Loader.h"
#include "GUIShader.h"
#include "Textures/GUITexture.h"
#include "Text/GUIText.h"
#include "Buttons/GUIButton.h"
#include "GUI.h"
using namespace std;
class CGUIRenderer
{	
public:
	~CGUIRenderer();
	void Init(int window_width, int window_height);
	void Render(const CGUI& gui) const;
	void InitGUI(CGUI& gui);

	GUIShader& GetGuiShader() { return m_Shader; }
	FontShader& GetFontShader() { return m_Fontshader; }
	CFont& GetFont() {	return m_TextFont;}

private:
	GUIShader m_Shader;
	glm::mat4 m_TransformationMatrix;

	shared_ptr<CGUITexture> m_Cursor;
	glm::vec2				m_WindowSize;

	//freetype::font_data m_TextFont;
	CFont				m_TextFont;
	FontShader			m_Fontshader;
	string				m_FontName;

	GLuint m_QuadVao, m_IndVbo, m_VertexVbo;
};
