#include "SceneEditor.h"

#ifdef EDITOR
void CSceneEditor::CreateObjectsTree()
{
	m_Hwnd[Hwnds::OBJECT_TREEE] = CreateWindowEx(WS_EX_COMPOSITED, WC_TREEVIEW, "ObjectTree",
		WS_CHILD | WS_VISIBLE  | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
		5, 5, m_Width / 2 - 330, 445, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::OBJECT_TREEE, m_Instance, NULL);

	TreeView_SetBkColor(m_Hwnd[Hwnds::OBJECT_TREEE], BKCOLOR);
	TreeView_SetTextColor(m_Hwnd[Hwnds::OBJECT_TREEE], TEXTCOLOR);
	TreeView_SetLineColor(m_Hwnd[Hwnds::OBJECT_TREEE], TEXTCOLOR);

	HIMAGELIST himl;
	HBITMAP hbmp;
	SHORT nLeaf, nNodeClosed, nNodeOpen;

	himl = ImageList_Create(32, 32, ILC_COLOR32, 4, 0);
	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/camera.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	nLeaf = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/entity.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	nNodeClosed = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/terrain.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	nNodeOpen = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/light.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	nNodeOpen = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	TreeView_SetImageList(m_Hwnd[Hwnds::OBJECT_TREEE], himl, TVSIL_NORMAL);

}
void CSceneEditor::FillObjectsTree()
{
	TreeView_DeleteAllItems(m_Hwnd[Hwnds::OBJECT_TREEE]);

	TVITEM tvi;
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.pszText = "Element";
	tvi.iImage = tvi.iSelectedImage = 0;

	TVINSERTSTRUCT tvins;
	tvins.item = tvi;
	tvins.hParent = tvins.hInsertAfter = TVI_ROOT;

	tvi.pszText = (LPSTR)"Main Camera";
	tvins.item = tvi;
	tvins.hParent = TVI_ROOT;	
	tvins.hInsertAfter = TVI_ROOT;
	HTREEITEM h_camera = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

	bool first = true;
	for (int y = 0; y < m_Game.GetCurrentScene()->m_TerrainsCount -1; y++)
	for (int x = 0; x < m_Game.GetCurrentScene()->m_TerrainsCount -1; x++)
	{
		CTerrain* terrain = m_Game.GetCurrentScene()->GetTerrain(x, y);
		if (terrain == nullptr)
			continue;

		if (!terrain->m_IsInit)
			continue;

		tvi.pszText = (LPSTR)terrain->GetNameWithId().c_str();
		tvi.iImage = tvi.iSelectedImage = 2;
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
		HTREEITEM h_terrain = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

		for (const std::shared_ptr<CEntity>& enity : terrain->m_TerrainEntities)
		{
			RecursiveSetEntity(enity, h_terrain);
		}
	}
	for (const CLight& light : m_Game.GetCurrentScene()->GetLights())
	{
		std::string name;
		switch (light.GetType())
		{
		case LightType::DIRECTIONAL_LIGHT:	name = "DIRECTIONAL_LIGHT"; break;
		case LightType::POINT_LIGHT:		name = "POINT_LIGHT"; break;
		case LightType::SPOT_LIGHT:			name = "SPOT_LIGHT"; break;
		}
		tvi.pszText = (LPSTR)name.c_str();
		tvi.iImage = tvi.iSelectedImage = 3;
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
		
		HTREEITEM h_light = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);
	}
	for (const std::shared_ptr<CEntity>& enity : m_Game.GetCurrentScene()->GetEntities())
	{
		tvi.pszText = (LPSTR)enity->GetNameWithID().c_str();
		tvi.iImage = tvi.iSelectedImage = 1;
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
		HTREEITEM h_entity = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

		for (const std::shared_ptr<CEntity>& child : enity->GetChildrenEntities())
		{
			RecursiveSetEntity(child, h_entity);
		}
	}
}

void CSceneEditor::RecursiveSetEntity(const std::shared_ptr<CEntity>& enity, HTREEITEM root)
{
	TVITEM tvi;
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.pszText = "Element";
	tvi.iImage = tvi.iSelectedImage = 1;

	TVINSERTSTRUCT tvins;
	tvins.item = tvi;

	tvi.pszText = (LPSTR)enity->GetNameWithID().c_str();
	tvins.item = tvi;
	tvins.hInsertAfter = TVI_FIRST;
	tvins.hParent = root;
	HTREEITEM h_entity = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

	for (const std::shared_ptr<CEntity>& child : enity->GetChildrenEntities())
	{
		RecursiveSetEntity(child, h_entity);
	}
}
bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
void CSceneEditor::TreeProcedure(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	
	switch (((LPNMHDR)lParam)->code)
	{
	

	case TVN_GETDISPINFO:
	{
		
	}
	break;

	case TVN_SELCHANGED:
	{		
		HTREEITEM hCurrentItem = TreeView_GetSelection(m_Hwnd[Hwnds::OBJECT_TREEE]);
		if (hCurrentItem == NULL) // Nothing selected
			break;
		// Now get the text of the selected item
		TCHAR buffer[128];

		TVITEM item;
		item.hItem = hCurrentItem;
		item.mask = TVIF_TEXT;
		item.cchTextMax = 128;
		item.pszText = buffer;
		if (TreeView_GetItem(m_Hwnd[Hwnds::OBJECT_TREEE], &item))
		{
			string text(item.pszText);
			std::string a_id = text.substr(text.find_last_of('_') + 1);
			std::string id = a_id.substr(a_id.find_first_not_of(a_id[0]));
			switch (a_id[0])
			{
			case 't':
				m_CurrentEntity = nullptr;
				if (is_number(id))
				{
					m_CurrentTerrain = m_Game.GetCurrentScene()->FindTerrainById(stoi(id));
					SetWindowText(m_Hwnd[Hwnds::INSPECTOR_TEXT_CURRENT_SELECTED], m_CurrentTerrain->GetNameWithId().c_str());
					SetWindowText(m_Hwnd[Hwnds::INSPECTOR_PAINT_STRENGTH], to_string(m_Game.GetCurrentScene()->m_HeightPaintStrength).c_str());
					SetWindowText(m_Hwnd[Hwnds::INSPECTOR_PAINT_SIZE], to_string(m_Game.GetCurrentScene()->m_BrushSize).c_str());
					m_Game.GetCurrentScene()->m_CurrentTerrain = m_CurrentTerrain;
				}
				break;
			case 'e':
				m_CurrentTerrain = nullptr;
				if (is_number(id))
				{
					m_CurrentEntity = m_Game.GetCurrentScene()->FindEntity(stoi(id));
					if (m_CurrentEntity == nullptr) break;
					
					SetWindowText(m_Hwnd[Hwnds::INSPECTOR_TEXT_ATTACHED_OFFSET], std::to_string(m_CurrentEntity->GetAttachYOffset()).c_str());
					PostMessage(m_Hwnd[Hwnds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT], BM_SETCHECK, BST_UNCHECKED, 0);
					m_AttachToTerrainHeight = false;	
					SetWindowText(m_Hwnd[Hwnds::INSPECTOR_TEXT_CURRENT_SELECTED], m_CurrentEntity->GetNameWithID().c_str());
				}				
				break;
			}
		}

	}
	break;

	}
}
#endif