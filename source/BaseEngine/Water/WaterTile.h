#pragma once
#include "glm/glm.hpp"
#include "../Utils/Utils.h"
#include "../Loader/Texture.h"
#include "../Loader/OpenGLObject.h"
#include "../Engine/GameObject.h"

class CWaterTile : public COpenGLObject, public CGameObject
{
public:
	CWaterTile();
	CWaterTile(glm::vec3 position, float size, float wave_speed, CTextInfo* dudv_texture, CTextInfo* normal_texture, glm::vec4 color = glm::vec4(0.0, 112.f / 255.f, 1, 0.2));
	~CWaterTile();
	void Init(glm::vec3 position, float size, float wave_speed, CTextInfo* dudv_texture, CTextInfo* normal_texture, glm::vec4 color = glm::vec4(0.0, 112.f / 255.f, 1, 0.2));
	void Update(const float& delta_time);
	
	const GLuint GetDudvTexture() const;
	const GLuint GetNormalTexture() const;

	const float& GetSize() const;
	const float& GetWaveSpeed() const;
	const float& GetMoveFactor() const;
	
	const glm::vec4& GetColor() const;
	const glm::vec3& GetPosition() const;
	const glm::mat4& GetTransformMatrix()const;

	const int& GetIndicesSize() const;
	const GLuint& GetMeshVao() const;

	// Inherited via COpenGLObject
	virtual void OpenGLLoadingPass() override;
private:
	glm::vec4 m_Color;

	int	   m_QuadIndicesSize;
	GLuint m_QuadVao;
	GLuint m_QuadIndices;
	GLuint m_QuadVertex;
	GLuint m_QuadTexCoord;

	CTextInfo* m_DudvTexture;
	CTextInfo* m_NormalMap;

	float		m_WaveSpeed;
	glm::vec3	m_Position;
	float		m_Size;
	float		m_MoveFactor;

	glm::mat4  m_TransformMatrix;

	bool m_IsInitialized = false;


};