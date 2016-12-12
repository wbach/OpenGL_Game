#include "Texture.h"

void CTextInfo::OpenGLLoadingPass()
{
	if (m_IsCubeMap)
	{
		SetCubMapTexture();
		return;
	}
	if (m_Data == nullptr || m_IsInit)
		return;
	// Create one OpenGL texture
	glGenTextures(1, &m_Id);
	GLenum hubo_error = glGetError();

	if (hubo_error)
	{
		delete[] m_Data;
		std::string error = "[Error] OGL There was an error loading the texture : " + filename + "\n";
		throw std::runtime_error(error.c_str());
	}
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_Id);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)m_Data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if(!m_KeepData)
		delete[] m_Data;
	m_Data = nullptr;
	m_IsInit = true;
}

void CTextInfo::SetCubMapTexture()
{
	if (m_CubeMapData[5] == nullptr || m_IsInit)
		return;

	glGenTextures(1, &m_Id);
	GLenum hubo_error = glGetError();
	
	if (hubo_error)
	{
		delete[] m_Data;
		std::string error = "[Error] OGL There was an error loading the texture : " + filename + "\n";
		throw std::runtime_error(error.c_str());
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);
	for (int x = 0;  x < 6; x++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + x, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)m_CubeMapData[x]);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (m_CubeMapData[x] != nullptr)
		{
			delete[] m_CubeMapData[x];
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	m_IsInit = true;
}

const GLuint & CTextInfo::GetId() const
{
	return m_Id;
}

CTextInfo::CTextInfo()
	: type(0)
	, m_Id(0)
	, m_IsInit(false)
	, m_KeepData(false)
	, m_Data(nullptr)
	, m_IsCubeMap(false)
{
}


CTextInfo::~CTextInfo()
{
	if (!m_IsInit)
		return;
	glDeleteTextures(1, &m_Id);
}
