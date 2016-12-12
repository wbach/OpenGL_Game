#pragma once
#include "../Utils/Utils.h"
#include "../Loader/Texture.h"
#include "../Loader/TextureLoader.h"
#include "../Loader/OpenGLObject.h"
class CGrass : public COpenGLObject
{
public:
	~CGrass();
	void Init(const std::vector<glm::vec3>& positions, const int& texture);
	void Render(CTextureLoader& loader) const;
	virtual void OpenGLLoadingPass() override;

	float  m_ViewDistance = 70.f;
private:
	GLuint m_MeshVao;
	GLuint m_VertexVbo;
	int m_GrassTexture;	

	int m_VertexSize;

	bool m_IsInitialized = false;

	std::vector<float> grassvertex;
};