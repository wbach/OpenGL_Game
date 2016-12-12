#include "SceneEditor.h"
#ifdef EDITOR
void CSceneEditor::RegisterGUIEditorDialogClass()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = GUIDialogProc;
	wc.hInstance = m_Instance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszClassName = "GUIDialogClass";
	RegisterClassEx(&wc);
}

void CSceneEditor::CreateGUIDialog()
{
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);
	rect.left = (rect.right / 2) - (400 / 2);
	rect.top = (rect.bottom / 2) - (100 / 2);

	m_Hwnd[Hwnds::GUI_MAIN_DIALOG] = CreateWindowEx(WS_EX_TOPMOST, "GUIDialogClass", "GUIEditor",
		WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 300, 500, NULL, NULL, m_Instance, NULL);

	m_Hwnd[Hwnds::GUI_TREE_VIEW] = CreateWindowEx(WS_EX_COMPOSITED, WC_TREEVIEW, "ObjectTree",
		WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
		0, 0, 330, 445, m_Hwnd[Hwnds::GUI_MAIN_DIALOG], (HMENU)ControlsIds::GUI_TREE_VIEW, m_Instance, NULL);

	TreeView_SetBkColor(m_Hwnd[Hwnds::GUI_TREE_VIEW], BKCOLOR);
	TreeView_SetTextColor(m_Hwnd[Hwnds::GUI_TREE_VIEW], TEXTCOLOR);
	TreeView_SetLineColor(m_Hwnd[Hwnds::GUI_TREE_VIEW], TEXTCOLOR);
	
	
	HIMAGELIST himl;
	HBITMAP hbmp;
	SHORT nLeaf, nNodeClosed, nNodeOpen;

	himl = ImageList_Create(32, 32, ILC_COLOR32, 4, 0);
	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/texture.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	nLeaf = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/text.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	nNodeClosed = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/button.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	nNodeOpen = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/window.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	nNodeOpen = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	TreeView_SetImageList(m_Hwnd[Hwnds::GUI_TREE_VIEW], himl, TVSIL_NORMAL);

	SetWindowLongPtr(m_Hwnd[Hwnds::GUI_MAIN_DIALOG], GWLP_USERDATA, (long)this);

	SetFont();
	FillGUITree();
}

void CSceneEditor::FillGUITree()
{
	TreeView_DeleteAllItems(m_Hwnd[Hwnds::GUI_TREE_VIEW]);

	TVITEM tvi;
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.pszText = "Element";
	tvi.iImage = tvi.iSelectedImage = 0;

	TVINSERTSTRUCT tvins;
	tvins.item = tvi;
	tvins.hParent = tvins.hInsertAfter = TVI_ROOT;


	bool first = true;
	for (const auto& e : m_Game.GetGUIRenderer().GetAllElements())
	{
		tvi.pszText = (LPSTR)e->GetName().c_str();
		tvi.iImage = tvi.iSelectedImage = e->GetType() - 1;
		tvins.item = tvi;
		tvins.hParent = TVI_ROOT;
		if (first)
		{
			tvins.hInsertAfter = TVI_ROOT;
			first = false;
		}
		else
		{
			tvins.hInsertAfter = TVI_LAST;
		}

		HTREEITEM h_light = TreeView_InsertItem(m_Hwnd[Hwnds::GUI_TREE_VIEW], &tvins);

		for (const auto& se : e->GetSubElements())
		{
			TVITEM tvi;
			tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvi.pszText = "Element";
			tvi.iImage = tvi.iSelectedImage = se->GetType() - 1;

			TVINSERTSTRUCT tvins;
			tvins.item = tvi;

			tvi.pszText = (LPSTR)se->GetName().c_str();
			tvins.item = tvi;
			tvins.hInsertAfter = TVI_FIRST;
			tvins.hParent = h_light;
			HTREEITEM h_entity = TreeView_InsertItem(m_Hwnd[Hwnds::GUI_TREE_VIEW], &tvins);
		}
	}	
}

LRESULT CSceneEditor::GUIDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CSceneEditor* me = (CSceneEditor*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (me) return me->RealGUIDialogProc(hwnd, msg, wParam, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CSceneEditor::RealGUIDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_ACTIVATE:
		{

		}
		break;
		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case TVN_SELCHANGED:
				{
					HTREEITEM hCurrentItem = TreeView_GetSelection(m_Hwnd[Hwnds::GUI_TREE_VIEW]);
					// Now get the text of the selected item
					TCHAR buffer[128];

					TVITEM item;
					item.hItem = hCurrentItem;
					item.mask = TVIF_TEXT;
					item.cchTextMax = 128;
					item.pszText = buffer;
					if (TreeView_GetItem(m_Hwnd[Hwnds::GUI_TREE_VIEW], &item))
					{
						string text(item.pszText);
						cout << text << endl;
						CGUIElement* e = m_Game.GetGUIRenderer().GetElement(text);
						if(e != nullptr)
							m_EditorScene->m_GUICreator.SetElementToEdit(e);
					}					
				}
				break;
			}
			
		}break;
	
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case ControlsIds::SPAWN_ENITY_DIALOG_COMBO:
				switch (HIWORD(wParam))
				{
				case CBN_DROPDOWN:

					break;
				}
				break;
			}
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
#endif