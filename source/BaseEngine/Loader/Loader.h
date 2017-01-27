#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <stdlib.h>
#include "../Utils/Utils.h"
#include "TextureLoader.h"
#include "Model.h"
#include "AssimpModel.h"
#include "FbxModel.h"
#include "OpenGLLoader.h"
using namespace std;

class CLoader
{
public:
	CLoader();
	~CLoader();
	vector<shared_ptr<CModel>>		m_Models;
	vector<shared_ptr<CTextInfo>>	m_Textures;
	
	void 	AddObjectToOpenGLLoadingPass(COpenGLObject* obj);
	COpenGLObject*	GetObjectToOpenGLLoadingPass();
	void 	AddObjectToOpenGLPostLoadingPass(COpenGLObject* obj);
	COpenGLObject*	GetObjectToOpenGLPostLoadingPass();


	void	AddModelToUpdate(const unsigned int& eid, const unsigned int& mid);
	void	UpdateModels(float delta_time);	
	int		LoadMesh(string file_name, bool time_update = false, bool skip_if_exist = true);
	int		LoadTexture(string file_name, bool vertical_flip = false);
	int		LoadTextureImmediately(string file_name, bool vertical_flip = false, bool fullresolution = false);
	int		LoadFullTexture(string file_name, GLubyte *&data, int &width, int &height, bool vertical_flip = false, bool horizontal_flip = false);
	int		LoadCubeMap(const vector<string>& filenames);
	void	CreateEmptyImage(const std::string filename, int width, int height);
	void	ReloadTexture(string file_name, GLuint& texture_id);
	void	ReloadTexture(GLubyte *data, GLuint& texture_id, int width, int height);
	void	UpdateSubTexture(CTextInfo& texture, GLubyte* subdata, int start_x, int start_y, int width, int height);
	std::vector<SFace> LoadFaces(const std::string& filename) const;
	CTextureLoader& GetTextureLoader();
	void	SaveTextureToFile(string file_name, GLubyte *data, int width, int height);
	void	SetMaxTextureResolution(const glm::vec2& resolution);
private:
	COpenGLLoader   m_OpenGLLoader;
	CTextureLoader	m_TextureLoader;
	unordered_map<int, int>   m_IndexesInFrame;
	vector<int>	m_IndexesUpdatingModels;
	bool m_UpdateThreadRun;
};

