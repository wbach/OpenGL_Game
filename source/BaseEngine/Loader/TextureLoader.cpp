#include "TextureLoader.h"

CTextureLoader::CTextureLoader(vector<shared_ptr<CTextInfo>>& textures_vector, COpenGLLoader& openGLLoader)
	: m_Textures(textures_vector)
	, m_OpenGLLoader(openGLLoader)
	, m_MaxTextureResolution(4096)
{
}

void CTextureLoader::ReadImage(string file_name, GLubyte *&data, int& width, int& height, bool vertical_flip, bool horizontal_flip, bool fullresolution)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(file_name.c_str(), 0);

	if (formato == FIF_UNKNOWN) 
	{ 
		std::string error = "[Error] wrong image format or file does not exist." + file_name + "\n";
		throw std::runtime_error(error.c_str());
		return;
	}

	FIBITMAP* imagen2 = FreeImage_Load(formato, file_name.c_str());
	if (!imagen2) 
	{ 
		std::string error = "[Error] wrong image format or file does not exist." + file_name + "\n";
		throw std::runtime_error(error.c_str());
		return;
	}

	FIBITMAP* imagen = FreeImage_ConvertTo32Bits(imagen2);
	if (!imagen)
	{
		FreeImage_Unload(imagen2);
		throw std::runtime_error(std::string("[Error] Cant convert to 32 bits.\n").c_str());
		return;
	}
//	FreeImage_Unload(imagen2);

	if (vertical_flip)
		FreeImage_FlipVertical(imagen);
	if (horizontal_flip)
		FreeImage_FlipHorizontal(imagen);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	if (!fullresolution)
	{
		bool resize = false;
		if (w > static_cast<int>(m_MaxTextureResolution.x))
		{
			w = static_cast<int>(m_MaxTextureResolution.x);
			resize = true;
		}
		if (h > static_cast<int>(m_MaxTextureResolution.y))
		{
			h = static_cast<int>(m_MaxTextureResolution.y);
			resize = true;
		}

		if (resize)
		{
			imagen = FreeImage_Rescale(imagen, w, h, FILTER_BSPLINE);
		}
	}
	
	width = w;
	height = h;

	char* pixeles = (char*)FreeImage_GetBits(imagen);

	data = new GLubyte[4 * w*h];

	//bgr2rgb
	for (int j = 0; j<w*h; j++)
	{
		data[j * 4 + 0] = pixeles[j * 4 + 2];
		data[j * 4 + 1] = pixeles[j * 4 + 1];
		data[j * 4 + 2] = pixeles[j * 4 + 0];
		data[j * 4 + 3] = pixeles[j * 4 + 3];
	}
	FreeImage_Unload(imagen);
	FreeImage_Unload(imagen2);
}

int CTextureLoader::LoadFullTexture(string file_name, bool keepData, GLubyte *&texture, int &width, int &height, bool immediately, bool vertical_flip, bool horizontal_flip, bool fullresolution)
{
	string file = file_name.substr(file_name.find_last_of('/') + 1);
	
	int i = 0;
	for (auto& t : m_Textures)
	{
		if (file.compare(t->filename) == 0)
			return i;
		i++;
	}
	try
	{
		ReadImage(file_name, texture, width, height, vertical_flip, horizontal_flip, fullresolution);
	}
	catch (const std::exception& e)
	{
		throw e;
	}

	//if (!keepData)
	//	delete[] texture;
	shared_ptr<CTextInfo> nt(new CTextInfo());
	nt->m_Data = texture;
	nt->m_KeepData = keepData;
	nt->m_Width = width;
	nt->m_Height = height;
	nt->filename = file;
	nt->fullpath = file_name;

	if (!immediately)
		m_OpenGLLoader.AddObjectToOpenGLLoadingPass(nt.get());
	else
		nt->OpenGLLoadingPass();

	m_Textures.push_back(nt);
	return m_Textures.size() -1;
}

int CTextureLoader::LoadTexture(string filename, bool immediately, bool verticalFlip, bool fullresolution)
{
	int w, h; GLubyte* data = nullptr;
	try 
	{
		return LoadFullTexture(filename, false, data, w, h, immediately, verticalFlip, false, fullresolution);
	}
	catch (const std::exception& e)
	{
		throw e;
	}	
}

int CTextureLoader::LoadCubeMap(const vector<string>& filenames)
{
	shared_ptr<CTextInfo> nt(new CTextInfo());
	m_Textures.push_back(nt);
	int i = 0;
	for (const auto& file : filenames)
	{
		try
		{
			ReadImage(file, nt->m_CubeMapData[i++], nt->m_Width, nt->m_Height, true, true);
		}
		catch (const std::exception& e)
		{
			throw e;
		}	
	}
	nt->m_IsCubeMap = true;
	nt->filename = "CubeMap";
	m_OpenGLLoader.AddObjectToOpenGLLoadingPass(nt.get());
	return m_Textures.size() - 1;
}

void CTextureLoader::CreateEmptyImage(const std::string file_name, int width, int height)
{
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32, 8, 8, 8);

	width = FreeImage_GetWidth(bitmap);
	height = FreeImage_GetHeight(bitmap);

	for (int y = height - 1; y >= 0; y--)
	{
		BYTE *bits = FreeImage_GetScanLine(bitmap, height - 1 - y);
		for (int x = 0; x < width; x++)
		{
			bits[0] = 0;
			bits[1] = 0;
			bits[2] = 0;
			bits[3] = 255;
			bits += 4;
		}
	}

	FreeImage_FlipVertical(bitmap);
	FreeImage_Save(FIF_PNG, bitmap, file_name.c_str(), PNG_DEFAULT);
	FreeImage_Unload(bitmap);
}

void CTextureLoader::ReloadTexture(string file_name, GLuint* texture_id)
{
	cout << "1";
	glDeleteTextures(1, texture_id);
	try 
	{
		//*texture_id = LoadTexture(file_name)->GetId();
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

void CTextureLoader::ReloadTexture(GLubyte * data, GLuint& texture_id, int width , int height)
{
	glDeleteTextures(1, &texture_id);
	// Create one OpenGL texture
	glGenTextures(1, &texture_id);
	GLenum hubo_error = glGetError();

	if (hubo_error)
	{
		throw std::runtime_error(std::string("[Error] Reload. There was an error reloading the texture\n").c_str());
	}
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);

	//float max_t;
	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_t);
	//float amount = min(4.f, max_t);
	//glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, amount);

}

void CTextureLoader::UpdateSubTexture(CTextInfo & texture, GLubyte * subdata, int start_x, int start_y, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, texture.GetId());
	glTexSubImage2D(GL_TEXTURE_2D, 0, start_x, start_y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)subdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CTextureLoader::SaveTextureToFile(string file_name, GLubyte* data, int width, int height)
{
	//FIBITMAP* Image = FreeImage_ConvertFromRawBits(data, width, height, 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32, 8, 8, 8);

	width = FreeImage_GetWidth(bitmap);
	height = FreeImage_GetHeight(bitmap);

	for (int y = height - 1; y >= 0; y--)
	{
		BYTE *bits = FreeImage_GetScanLine(bitmap, height - 1 - y);
		for (int x = 0; x < width; x++)
		{
			bits[0] = data[(y*width + x) * 4 + 2];
			bits[1] = data[(y*width + x) * 4 + 1];
			bits[2] = data[(y*width + x) * 4 + 0];
			bits[3] = data[(y*width + x) * 4 + 3];
			bits += 4;

		}

	}
	FreeImage_FlipVertical(bitmap);
	FreeImage_Save(FIF_PNG, bitmap, file_name.c_str(), PNG_DEFAULT);
	FreeImage_Unload(bitmap);
}

shared_ptr<CTextInfo> & CTextureLoader::GetTexture(uint id)
{
	return m_Textures[id];
}

void CTextureLoader::SetMaxTextureResolution(const glm::vec2 & resolution)
{
	m_MaxTextureResolution = resolution;
}

//COpenGLObject* CTextureLoader::GetTextureToOpenGLLoad()
//{
//	std::lock_guard<std::mutex> lock(m_Mutex);
//	if (m_OpenGLPassLoad.empty())
//		return nullptr;
//	COpenGLObject* t = m_OpenGLPassLoad.back();
//	m_OpenGLPassLoad.pop_back();
//	return t;
//}


