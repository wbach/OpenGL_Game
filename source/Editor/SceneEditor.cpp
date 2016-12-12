#include "SceneEditor.h"

#ifdef EDITOR
CSceneEditor::CSceneEditor(CGame& game)
	: m_Game(game)
	, m_CurrentEntity(nullptr)
{
	m_Instance = GetModuleHandle(NULL);

	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	m_CurrentPath = buffer;
	m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.find_last_of('\\'));
	m_CurrentPath += '\\';
}

void CSceneEditor::Init(int width, int height)
{
	m_Width = width;
	m_Height = height;

	//INITCOMMONCONTROLSEX icc;
	//icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//icc.dwICC = ICC_BAR_CLASSES; // toolbary, statusbary, tooltipy i oczywiœcie progressbary
	InitCommonControls();

	// Create Editro window
	CreateWndClass();
	CreateMainWindow();
	
	hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// Attach OpenGL window
	SetWindowLongPtr(m_Hwnd[Hwnds::MAIN_WINDOW], GWLP_USERDATA, (long)this);
	AttachOpenGLWindow();
	
	// file explorer init
	CreateFileList();	
	FindFilesInDirectory(m_CurrentPath);
	CheckAvaibleDrives();
	
	// Objects Tree init
	CreateObjectsTree();
	FillObjectsTree();

	//Create Inspector
	CreateInspector(m_Width - 175, 5);

	RegisterGUIEditorDialogClass();
	CreateGUIDialog();
	SetFont();

	ShowWindow(m_Hwnd[Hwnds::MAIN_WINDOW], SW_SHOWDEFAULT); // Poka¿ okienko...
	UpdateWindow(m_Hwnd[Hwnds::MAIN_WINDOW]);

}
void CSceneEditor::AddMenus() 
{
	m_Menus[MenuHandles::POUP] = CreateMenu();
	m_Menus[MenuHandles::ELEMENT_FILE] = CreateMenu();
	m_Menus[MenuHandles::ELEMENT_HELP] = CreateMenu();

	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_STRING, ControlsIds::MENU_NEW_SCENE, L"&New");
	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_STRING, ControlsIds::MENU_OPEN_SCENE, L"&Open");
	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_STRING, ControlsIds::MENU_SAVE_OBJECTS, L"&Save objects");
	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_STRING, ControlsIds::MENU_SAVE_BLENDMAPS, L"&Save blend maps");
	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_STRING, ControlsIds::MENU_SAVE_HEIGHTMAPS, L"&Save heightmaps");
	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_SEPARATOR, 0, NULL);
	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_STRING, ControlsIds::MENU_QUIT, L"&Quit");

	AppendMenuW(m_Menus[MenuHandles::ELEMENT_HELP], MF_STRING, ControlsIds::MENU_ABOUT, L"&About");

	AppendMenuW(m_Menus[MenuHandles::POUP], MF_POPUP, (UINT_PTR) m_Menus[MenuHandles::ELEMENT_FILE], L"&File");
	AppendMenuW(m_Menus[MenuHandles::POUP], MF_POPUP, (UINT_PTR) m_Menus[MenuHandles::ELEMENT_HELP], L"&About");

	

	SetMenu(m_Hwnd[Hwnds::MAIN_WINDOW], m_Menus[MenuHandles::POUP]);
}
void CSceneEditor::CheckActiveWindows()
{

}
void CSceneEditor::SetFont()
{
	// Setfont to all elements
	for (int x = 0; x < Hwnds::COUNT; x++)
	{
		SendMessage(m_Hwnd[x], WM_SETFONT, (WPARAM)hNormalFont, 0);
	}
}
void CSceneEditor::CreateMainWindow()
{
	// create window
	m_Hwnd[Hwnds::MAIN_WINDOW] = CreateWindowEx(WS_EX_WINDOWEDGE, m_ClassName, "Editor", WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height, NULL, NULL, m_Instance, NULL);

	if (m_Hwnd[Hwnds::MAIN_WINDOW] == NULL)
	{
		MessageBox(NULL, "Cant create editor window.", "Editor", MB_ICONEXCLAMATION);
		return;
	}
	AddMenus();
	RegisterLoadingClass(m_Hwnd[Hwnds::MAIN_WINDOW]);
//	CreateDialogProgressBar();
}

void CSceneEditor::AttachOpenGLWindow()
{
	m_Hwnd[Hwnds::OPENGL_WINDOW] = FindWindow(NULL, "Gota - OpenGL");
	SetParent(m_Hwnd[Hwnds::OPENGL_WINDOW], m_Hwnd[Hwnds::MAIN_WINDOW]);
	SetWindowPos(m_Hwnd[Hwnds::OPENGL_WINDOW], 0, m_Width / 2 - 320, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void CSceneEditor::CreateWndClass()
{
	// Create wnd class
	m_Wc.cbSize = sizeof(WNDCLASSEX);
	m_Wc.style = CS_GLOBALCLASS | CS_BYTEALIGNCLIENT;
	m_Wc.lpfnWndProc = WndProc;
	m_Wc.cbClsExtra = 0;
	m_Wc.cbWndExtra = 0;
	m_Wc.hInstance = m_Instance;
	m_Wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_Wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_Wc.hbrBackground = CreateSolidBrush(BKCOLOR); //(HBRUSH)(COLOR_WINDOW + 1);// //
	m_Wc.lpszMenuName = NULL;
	m_Wc.lpszClassName = m_ClassName;
	m_Wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// REJESTROWANIE KLASY OKNA
	if (!RegisterClassEx(&m_Wc))
	{
		MessageBox(NULL, "Cant register window", "Error",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}
}
void CSceneEditor::CreateDialogProgressBar()
{
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);
	rect.left = (rect.right / 2) - (400 / 2);
	rect.top = (rect.bottom / 2) - (100 / 2);
	
	 s_DialogLoading = m_Hwnd[Hwnds::LOADING_DIALOG] = CreateWindowExW(WS_EX_TOPMOST, L"LoadingDialogClass", L"Loading Scene",
		WS_VISIBLE  | WS_POPUP, rect.left, rect.top, 400, 30,
		NULL, NULL, m_Instance, NULL);

	 s_ProgresBarLoading = m_Hwnd[Hwnds::LOADING_PROGRESS_BAR] = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH ,
		0, 0, 400, 30, m_Hwnd[Hwnds::LOADING_DIALOG], (HMENU)200, m_Instance, NULL);

	//SendMessage(m_Hwnd[Hwnds::LOADING_PROGRESS_BAR], PBM_SETMARQUEE, TRUE, 0);

}
void CSceneEditor::DeleteDialogProgressBar()
{
	CloseWindow(m_Hwnd[Hwnds::LOADING_DIALOG]);
}
void CSceneEditor::RegisterLoadingClass(HWND hwnd)
{
	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.lpfnWndProc = (WNDPROC)LoadingDialogProc;
	wc.hInstance = m_Instance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszClassName = L"LoadingDialogClass";
	RegisterClassExW(&wc);
}
LRESULT CSceneEditor::LoadingDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_CREATE:
		break;

	case WM_COMMAND:
		DestroyWindow(hwnd);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	}
 
	return(DefWindowProcW(hwnd, msg, wParam, lParam));
}

std::string CSceneEditor::GetTextFromControl(HWND hwnd) const
{
	DWORD dlugosc = GetWindowTextLength(hwnd);
	LPSTR Bufor = (LPSTR)GlobalAlloc(GPTR, dlugosc + 1);
	GetWindowText(hwnd, Bufor, dlugosc + 1);
	return std::string(Bufor);
}
string FloatToString3(const float& x)
{
	char text[20];
	sprintf_s(text, "%.3f", x);
	return string(text);
}
void CSceneEditor::UpdateValueControl(HWND hwnd, const float& x) const
{
	HWND focus = GetFocus();
	if (FloatToString3(x).compare(GetTextFromControl(hwnd)) != 0 && focus != hwnd)
		SetWindowText(hwnd, (LPCSTR)FloatToString3(x).c_str());
}
void CSceneEditor::PeekMesages()
{	
	if (m_CurrentEntity != nullptr)
	{
		UpdateInspector();
		m_Game.GetCurrentScene()->SetEntityToMousePointByKey(m_CurrentEntity);
	}
}
//*

void CSceneEditor::DisplayBitmap(HWND hwnd, HBITMAP bitmap, HDC hdc, int x, int y)
{

	BITMAP bm;

	GetObject(bitmap, sizeof(bm), &bm);

	HDC hdcMem = CreateCompatibleDC(hdc);

	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, bitmap);

	BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	DeleteObject(hdcMem);
}

HBITMAP CSceneEditor::Create8bppBitmap(HDC hdc, int width, int height, LPVOID pBits)
{
	BITMAPINFO *bmi = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
	BITMAPINFOHEADER &bih(bmi->bmiHeader);
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = width;
	bih.biHeight = -height;
	bih.biPlanes = 1;
	bih.biBitCount = 8;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = 0;
	bih.biXPelsPerMeter = 14173;
	bih.biYPelsPerMeter = 14173;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
	for (int I = 0; I <= 255; I++)
	{
		bmi->bmiColors[I].rgbBlue = bmi->bmiColors[I].rgbGreen = bmi->bmiColors[I].rgbRed = (BYTE)I;
		bmi->bmiColors[I].rgbReserved = 0;
	}

	void *Pixels = NULL;
	HBITMAP hbmp = CreateDIBSection(hdc, bmi, DIB_RGB_COLORS, &Pixels, NULL, 0);

	if (pBits != NULL)
	{
		//fill the bitmap
		BYTE* pbBits = (BYTE*)pBits;
		BYTE *Pix = (BYTE *)Pixels;
		memcpy(Pix, pbBits, width * height);
	}

	free(bmi);

	return hbmp;
}

HBITMAP CSceneEditor::CreateBitmapFromPixels(HDC hDC, UINT uWidth, UINT uHeight, UINT uBitsPerPixel, LPVOID pBits)
{
	if (uBitsPerPixel < 8) // NOT IMPLEMENTED YET
		return NULL;

	if (uBitsPerPixel == 8)
		return Create8bppBitmap(hDC, uWidth, uHeight, pBits);

	HBITMAP hBitmap = 0;
	if (!uWidth || !uHeight || !uBitsPerPixel)
		return hBitmap;
	LONG lBmpSize = uWidth * uHeight * (uBitsPerPixel / 8);
	BITMAPINFO bmpInfo = { 0 };
	bmpInfo.bmiHeader.biBitCount = uBitsPerPixel;
	bmpInfo.bmiHeader.biHeight = uHeight;
	bmpInfo.bmiHeader.biWidth = uWidth;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Pointer to access the pixels of bitmap
	UINT * pPixels = 0;
	hBitmap = CreateDIBSection(hDC, (BITMAPINFO *)&
		bmpInfo, DIB_RGB_COLORS, (void **)&
		pPixels, NULL, 0);

	if (!hBitmap)
		return hBitmap; // return if invalid bitmaps

						//SetBitmapBits( hBitmap, lBmpSize, pBits);
						// Directly Write
	memcpy(pPixels, pBits, lBmpSize);

	return hBitmap;
}
#endif