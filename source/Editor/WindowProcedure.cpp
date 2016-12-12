#include "SceneEditor.h"
#include "../TestGame/Test_Scene.h"
#ifdef EDITOR

LRESULT CSceneEditor::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CSceneEditor* me = (CSceneEditor*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (me) return me->RealWndProc(hwnd, msg, wParam, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}




LRESULT CSceneEditor::RealWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		m_Game.m_FroceQuit = true;
		DestroyWindow(hwnd);
		break;
	case WM_NOTIFY:
		FileExplorerProcedure(wParam, lParam);	
		TreeProcedure(hwnd, wParam,lParam);
		TexturePaintProcedure( wParam, lParam);
		break;
	case WM_COMMAND:
		InspectorProcedure(LOWORD(wParam), HIWORD(wParam));
		switch (LOWORD(wParam))
		{
		case ControlsIds::DRIVE_COMBO:
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				break;
			case BN_CLICKED:
				break;
			case CBN_SELCHANGE:
				//int numer = SendMessage(m_Hwnd[Hwnds::DRIVE_COMBO], CB_GETCURSEL, 0, 0);
				char buf[10];
				GetDlgItemText(hwnd, ControlsIds::DRIVE_COMBO, buf, 9);
				//MessageBox(NULL, buf, NULL, MB_OK);
				m_CurrentPath = std::string(buf);
				FindFilesInDirectory(m_CurrentPath);
				break;
			}
			break;
		case ControlsIds::MENU_NEW_SCENE:
			break;		
		case  ControlsIds::MENU_OPEN_SCENE:
		{
			OPENFILENAME ofn;
			char sNazwaPliku[MAX_PATH] = "";

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = "Pliki mapy (*.map)\0*.map\0Wszystkie pliki\0*.*\0";
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFile = sNazwaPliku;
			ofn.lpstrDefExt = "map";
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			bool a = false;
			char current_directory[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, current_directory);
			if (GetOpenFileName(&ofn))
			{
				std::string window_title = ("Editor:          ") + string(sNazwaPliku);
				SetWindowText(m_Hwnd[Hwnds::MAIN_WINDOW], window_title.c_str());
				SetCurrentDirectory(current_directory);
				//GetCurrentDirectory(MAX_PATH, dar);
			//	CreateDialogProgressBar();
				//CreateDialogProgressBar();
				//m_Game.GetDisplayManager().DisableTime();
				//m_Game.GetCurrentScene()->Reset();
				////m_Game.ClearScenes();
				////m_Game.AddScene(std::make_shared<CTestSCene>(m_Game, 1));
				////m_Game.SetCurrentScene(0);
				//m_Game.m_SceneParser.LoadScene(sNazwaPliku, m_Game.GetCurrentScene());
				//m_Game.GetCurrentScene()->PostInitialize();
				//m_Game.GetDisplayManager().EnableTime();

				//m_Game.GetCurrentScene()->ClearObjectsVelocity();

				//m_Game.m_SceneParser.LoadScene(sNazwaPliku, m_Game.GetCurrentScene(), [](int p) {
				//	//cout << p << endl;  
				//	//SendMessage(s_ProgresBarLoading, PBM_SETPOS, (WPARAM)p, 0); 				
				//});
				//FillObjectsTree();
			/*	m_CurrentPath = string(sNazwaPliku);
				m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.find_last_of("\\") +1);
				FindFilesInDirectory(m_CurrentPath);
				DeleteDialogProgressBar();*/
			}		
			
			break;
		}
		case  ControlsIds::MENU_SAVE_SCENE:
			m_Game.m_SceneParser.SaveToFile(m_CurrentPath, m_Game.GetCurrentScene());
			break;
		case ControlsIds::MENU_SAVE_HEIGHTMAPS:
			m_Game.GetCurrentScene()->SaveHeightMaps();			
				break;
		case ControlsIds::MENU_SAVE_BLENDMAPS:
			m_Game.GetCurrentScene()->SaveBlendMaps();
			break;
		case  ControlsIds::MENU_SAVE_OBJECTS:
		{
			OPENFILENAME ofn;
			char sNazwaPliku[MAX_PATH] = "";

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = "Pliki mapy (*.map)\0*.map\0Wszystkie pliki\0*.*\0";
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFile = sNazwaPliku;
			ofn.lpstrDefExt = "map";
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			bool a = false;
			char current_directory[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, current_directory);

			if (GetSaveFileName(&ofn))
			{
				SetCurrentDirectory(current_directory);
				CreateDialogProgressBar();
				m_Game.m_SceneParser.SaveToFile(sNazwaPliku, m_Game.GetCurrentScene());				
				DeleteDialogProgressBar();
			}
		}

			break;
		case  ControlsIds::MENU_QUIT:
			m_Game.m_FroceQuit = true;
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ControlsIds::MENU_ABOUT:

			break;
		}
		break;	
	case WM_DESTROY:
		m_Game.m_FroceQuit = true;
		PostQuitMessage(0);
		break;
	//case WM_ERASEBKGND:
	//{
	//	RECT rc;
	//	GetClientRect(hwnd, &rc);
	//	SetBkColor((HDC)wParam, RGB(30, 30, 30)); // red
	//	ExtTextOut((HDC)wParam, 0, 0, ETO_OPAQUE, &rc, 0, 0, 0);
	//	return 1;
	//}
	//// or in WM_PAINT
	//case WM_PAINT:
	//{
	//	PAINTSTRUCT ps;
	//	RECT rc;
	//	HDC hdc = BeginPaint(hwnd, &ps);
	//	GetClientRect(hwnd, &rc);
	//	SetBkColor(hdc, RGB(30, 30, 30)); // red
	//	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, 0, 0, 0);
	//	EndPaint(hwnd, &ps);
	//	break;
	//}
	//case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSTATIC:
	{
		HWND hCtl = (HWND)lParam;
		HDC hdc = (HDC)wParam;

		SetTextColor(hdc, TEXTCOLOR);
		SetBkColor(hdc, BKCOLOR);
		return (LRESULT)GetStockObject(NULL_BRUSH);
		
	}
	break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

#endif