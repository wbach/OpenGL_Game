#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include "OpenGLObject.h"
using namespace std;

namespace MaterialTexture
{
	enum {
		AMBIENT = 0,
		DIFFUSE,
		SPECULAR,
		NORMAL,
		REFLECTION,
		BLEND_MAP,
		GUI
	};
}

class CTextInfo : public COpenGLObject
{
public:
	virtual void OpenGLLoadingPass() override;
	void SetCubMapTexture();
	const GLuint& GetId() const;
	CTextInfo();
	~CTextInfo();

	string filename;
	string fullpath;
	unsigned int type;
	GLubyte* m_Data;
	GLubyte* m_CubeMapData[6];
	int m_Width, m_Height;
	bool m_KeepData;
	bool m_IsCubeMap; 
private:
	GLuint m_Id;
	bool m_IsInit;	
};