#pragma once
//#define EDITOR
#ifdef EDITOR
#include <Windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <iostream>
#include <strsafe.h>
#include <GL/glew.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <cctype>
#include "../BaseEngine/Engine/GLgame.h"
#include "../TestGame/Test_Scene.h"
#include <thread>
#include "HwndsList.h"
#include "IdsList.h"
#define TEXTCOLOR RGB(0, 0, 0)
#define BKCOLOR RGB(250, 250, 250)
#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

static HWND s_ProgresBarLoading;
static HWND s_DialogLoading;

namespace FilesTypes 
{
	enum Type
	{
		NONE = 0,
		DIRECTORY,
		DEFAULT,
		IMAGE,
		COUNT
	};
}

namespace MenuHandles
{
	enum Type
	{
		POUP = 0,
		ELEMENT_FILE,
		ELEMENT_HELP,
		COUNT
	};
}
struct SFile 
{
	FilesTypes::Type fileType;
	std::string name;
	std::string date;
	long int size;
};

class CSceneEditor
{
public:
	CSceneEditor(CGame& game);
	void Init(int width, int height);	
	void PeekMesages();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	

	CGameEditorScene* m_EditorScene;
private:	
	//
	void CreateDialogProgressBar();
	void DeleteDialogProgressBar();
	void RegisterLoadingClass(HWND hwnd);
	static LRESULT CALLBACK LoadingDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	
	std::string GetTextFromControl(HWND hwnd) const;

	// Inspector function
	void CreateInspector(int w, int h);
	void CreateTerrainTextureView(int &w, int &h, int size_x, int size_y);
	void CreateTransformView(int &w, int &h, int size_x, int size_y);
	void UpdateValueControl(HWND hwnd, const float& x) const;
	void InspectorProcedure(WPARAM wParam, LPARAM lParam);
	void UpdateInspector();
	void UpdateEntity(ControlsIds::Ids type);
	void GetValueFromControl(HWND hwnd, float& value);	
	void TexturePaintProcedure(WPARAM wParam, LPARAM lParam);

	//GUIEditor
	void RegisterGUIEditorDialogClass();
	void CreateGUIDialog();
	void FillGUITree();
	static LRESULT CALLBACK GUIDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK RealGUIDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Objects Tree functions
	void CreateObjectsTree();
	void FillObjectsTree();
	void RecursiveSetEntity(const std::shared_ptr<CEntity>& enity, HTREEITEM root);
	void TreeProcedure(HWND hwnd, WPARAM wParam, LPARAM lParam);

	// File Expllorer functions
	void CheckAvaibleDrives();
	void FindFilesInDirectory(std::string path);
	void FillFileList();
	void CreateFileList();
	void AddFile(int i, const SFile& file);
	void SpawnEntity(std::string file_name, glm::vec3 normalized_scale);
	void FileExplorerProcedure(WPARAM wParam, LPARAM lParam);
	void RegisterSpawEntityDialogClass();
	void CreateSpawEntityDialog();
	static LRESULT CALLBACK SpawnEntityDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK RealSpawnEntityDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Window functions
	void AttachOpenGLWindow();	
	void CreateWndClass();
	void CreateMainWindow();
	void AddMenus();
	void CheckActiveWindows();
	void SetFont();
	LRESULT CALLBACK RealWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//*
	void DisplayBitmap(HWND hwnd, HBITMAP bitmap, HDC hdc, int x, int y);
	static HBITMAP Create8bppBitmap(HDC hdc, int width, int height, LPVOID pBits = NULL);
	static HBITMAP CreateBitmapFromPixels(HDC hDC, UINT uWidth, UINT uHeight, UINT uBitsPerPixel, LPVOID pBits);

	// Inspector variables
	bool m_AttachToTerrainHeight = false;
	shared_ptr<CEntity> m_CurrentEntity = nullptr;
	CTerrain* m_CurrentTerrain = nullptr;
	int m_CurrentPaintColor = 0;
	int m_BrushSize = 50;
	float m_PaintStrength = 1.f;

	// File Expllorer variables
	std::string				 m_CurrentPath;
	std::vector<SFile>		 m_CurrentPathFiles;
	std::string				 m_SpawnedPathFile;
	std::vector<std::string> m_Drives;
	bool					 m_NormalizeSpawnedMesh = true;

	// Window variables
	int			m_Width;
	int			m_Height;
	MSG			m_Messages;
	LPSTR		m_ClassName = "Window class";
	HWND		m_Hwnd[Hwnds::COUNT];
	WNDCLASSEX	m_Wc;
	HINSTANCE	m_Instance;
	HMENU		m_Menus[MenuHandles::COUNT];
	// Fonts
	HFONT hNormalFont;
	// Game
	CGame& m_Game;

};
#endif