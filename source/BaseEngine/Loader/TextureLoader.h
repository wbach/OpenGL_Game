#pragma once
#include <string>
#include <vector>
#include <FreeImage.h>
#include "Material.h"
#include <algorithm>
#include <list>
#include "OpenGLLoader.h"
class CTextureLoader
{
public:
	CTextureLoader(vector<shared_ptr<CTextInfo>>& textures_vector, COpenGLLoader& openGLLoader);
	void ReadImage(string filename, GLubyte *&data, int &width, int &height, bool vertical_flip = false, bool horizontal_flip = false, bool fullresolution = false);
	int LoadFullTexture(string filename, bool keep_data, GLubyte *&data, int &width, int &height, bool immediately = false, bool vertical_flip = false, bool horizontal_flip = false, bool fullresolution = false);
	int LoadTexture(string filename, bool immediately = false, bool verticalFlip = false , bool fullresolution = false);
	int LoadCubeMap(const vector<string>& filenames);
	void CreateEmptyImage(const std::string filename, int width, int height);
	void ReloadTexture(string file_name, GLuint* texture_id);
	void ReloadTexture(GLubyte *data, GLuint& texture_id, int width, int height);
	void UpdateSubTexture(CTextInfo& texture, GLubyte* subdata, int start_x, int start_y, int width, int height);
	void SaveTextureToFile(string file_name, GLubyte *data, int width, int height);
	shared_ptr<CTextInfo>&	GetTexture(uint id);
	void SetMaxTextureResolution(const glm::vec2& resolution);
//	void AddTextureToOpenGLLoad(CTextInfo* t);
//	CTextInfo* GetTextureToOpenGLLoad();
private:
	vector<shared_ptr<CTextInfo>>& m_Textures;
	COpenGLLoader&   m_OpenGLLoader;
	glm::vec2 m_MaxTextureResolution;	
};