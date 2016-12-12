#include "GUIRenderer.h"

CGUIRenderer::~CGUIRenderer()
{
	glDeleteBuffers(1, &m_VertexVbo);
	glDeleteBuffers(1, &m_IndVbo);
	glDeleteVertexArrays(1, &m_QuadVao);
}

void CGUIRenderer::Init(int window_width, int window_height)
{
	vector<float> postions = {
		-1,1,
		-1,-1,
		1,1,
		1,-1 };
	m_QuadVao = Utils::CreateVao();
	m_VertexVbo = Utils::StoreDataInAttributesList(0, 2, postions);
	Utils::UnbindVao();

	m_Shader.Init();
	m_Fontshader.Init();
	//m_TextFont.init("Data/GUI/consola.ttf", 100);
	
	//textFont.init("CRYSISB.ttf", 50);//"bgothm.ttf"
	m_Cursor = nullptr;
	m_WindowSize.x = static_cast<float>(window_width);
	m_WindowSize.y = static_cast<float>(window_height);
	m_TextFont.Init("Data/GUI/georgia.ttf", m_WindowSize);
}

void CGUIRenderer::InitGUI(CGUI & gui)
{
	gui.Init(&m_Shader, &m_Fontshader, &m_TextFont, &m_QuadVao);
}


void CGUIRenderer::Render(const CGUI& gui) const
{
	for (const auto& t : gui.GetAllElements())
	{
		if(t->m_IsVisible)
			t->Render();
	}
}
