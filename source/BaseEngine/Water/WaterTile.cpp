#include "WaterTile.h"

CWaterTile::CWaterTile()
	: m_IsInitialized(false)
{
	m_NormalMap = nullptr;
	m_DudvTexture = nullptr;
}

CWaterTile::CWaterTile(glm::vec3 position, float size, float wave_speed, CTextInfo* dudv_texture, CTextInfo* normal_texture, glm::vec4 color)
	: m_IsInitialized(false)
{
	Init(position, size, wave_speed, dudv_texture, normal_texture, color);
}

CWaterTile::~CWaterTile()
{
	if (!m_IsInitialized) return;
	std::cout << "Destructor " << typeid(*this).name() << std::endl;
	Utils::DeleteQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord);
	//glDeleteTextures(1, m_NormalMap->GetId());
	//glDeleteTextures(1, &m_DudvTexture);
}

void CWaterTile::Init(glm::vec3 position, float size, float wave_speed, CTextInfo* dudv_texture, CTextInfo* normal_texture, glm::vec4 color)
{
	m_WaveSpeed = wave_speed;
	m_Position = position;
	m_Size = size;
	m_DudvTexture = dudv_texture;
	m_NormalMap = normal_texture;	
	m_TransformMatrix = Utils::CreateTransformationMatrix(m_Position, glm::vec3(-90, 0, 0), glm::vec3(m_Size));
	m_Color = color; 
}

void CWaterTile::Update(const float& delta_time)
{
	m_MoveFactor += m_WaveSpeed*delta_time;
	m_MoveFactor = fmod(m_MoveFactor, 1);
}

const GLuint CWaterTile::GetDudvTexture() const
{
	if (m_DudvTexture == nullptr)
		return 0;
	return m_DudvTexture->GetId();
}

const GLuint CWaterTile::GetNormalTexture() const
{
	if (m_NormalMap == nullptr)
		return 0;
	return m_NormalMap->GetId();
}

const glm::vec4 & CWaterTile::GetColor() const
{
	return m_Color;
}

const glm::vec3& CWaterTile::GetPosition() const
{
	return m_Position;
}

const float& CWaterTile::GetSize() const
{
	return m_Size;
}

const float& CWaterTile::GetWaveSpeed() const
{
	return m_WaveSpeed;
}

const float & CWaterTile::GetMoveFactor() const
{
	return m_MoveFactor;
}

const glm::mat4 & CWaterTile::GetTransformMatrix() const
{
	return m_TransformMatrix;
}

const int & CWaterTile::GetIndicesSize() const
{
	return m_QuadIndicesSize;
}

const GLuint & CWaterTile::GetMeshVao() const
{
	return m_QuadVao;
}

void CWaterTile::OpenGLLoadingPass()
{
	Utils::CreateQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord, m_QuadIndicesSize);
	m_IsInitialized = true;
}
