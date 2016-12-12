#include "SceneEditor.h"

#ifdef EDITOR

void CSceneEditor::CreateInspector(int w, int h)
{
	int edit_size_x = 50, edit_size_y = 20;
	
	CreateTransformView(w, h, edit_size_x, edit_size_y);
	
	h += 2 * edit_size_y;
	
	m_Hwnd[Hwnds::INSPECTOR_TEXT_ATTACHED_OFFSET_TEXT] = CreateWindowEx(0, "STATIC", "Attached Y offset", WS_CHILD | WS_VISIBLE, w, h, 150, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_TEXT_ATTACHED_OFFSET] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w, h + edit_size_y, edit_size_x + 25, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_TEXT_ATTACHED_OFFSET, m_Instance, NULL);

	h += 2 * edit_size_y;

	m_Hwnd[Hwnds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT]  = CreateWindowEx(0, "BUTTON", "Attach to terrain height", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		w, h, 150, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT, m_Instance, NULL);

	h +=  edit_size_y;

	m_Hwnd[Hwnds::INSPECTOR_TEXT_CURRENT_SELECTED] = CreateWindowEx(0, "STATIC", "Current selected :", WS_CHILD | WS_VISIBLE, w, h, 150, 20, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_DELETE_CURRENT_SELECTED] = CreateWindowEx(0, "BUTTON", "Delete current selected.", WS_CHILD | WS_VISIBLE,
		w, h + edit_size_y, 150, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_DELETE_CURRENT_SELECTED, m_Instance, NULL);

	h += edit_size_y;

	m_Hwnd[Hwnds::INSPECTOR_GO_CAMERA_TO_OBJECT] = CreateWindowEx(0, "BUTTON", "Camera to current selected.", WS_CHILD | WS_VISIBLE,
		w, h + edit_size_y, 150, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_GO_CAMERA_TO_OBJECT, m_Instance, NULL);
	
	h += 2 * edit_size_y;
	CreateTerrainTextureView(w, h, edit_size_x, edit_size_y);
}

void CSceneEditor::CreateTerrainTextureView(int & w, int & h, int size_x, int edit_size_y)
{
	//HBITMAP hbmTool = (HBITMAP)LoadImage(m_Instance, "Data/toolbmp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	//TBBUTTON tbb[4];

	//ZeroMemory(tbb, sizeof(tbb));
	//for (int i = 0; i < 4; ++i) {
	//	tbb[i].idCommand = i;
	//	tbb[i].iBitmap = tbb[i].iString = i;
	//	tbb[i].fsState = TBSTATE_ENABLED;
	//	tbb[i].fsStyle = TBSTYLE_BUTTON;
	//}
	//HWND hToolbar = CreateToolbarEx(m_Hwnd[Hwnds::MAIN_WINDOW], WS_CHILD | WS_VISIBLE, 500, 4, NULL, (UINT)hbmTool, tbb, 4,
	//	16, 16, 16, 16, sizeof(TBBUTTON));

	//h += edit_size_y;
	//m_Hwnd[Hwnds::INSPECTOR_TEXTURE_BACKGROUNG] = CreateWindowEx(0, "BUTTON", "Tx 1", WS_CHILD | WS_VISIBLE,
	//	w -5, h, 2*edit_size_y, 2*edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_TEXTURE_BACKGROUNG, m_Instance, NULL);

	//m_Hwnd[Hwnds::INSPECTOR_TEXTURE_RED] = CreateWindowEx(0, "BUTTON", "Tx 2", WS_CHILD | WS_VISIBLE,
	//	w - 5 + 2*edit_size_y, h, 2 * edit_size_y, 2 * edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_TEXTURE_RED, m_Instance, NULL);

	//m_Hwnd[Hwnds::INSPECTOR_TEXTURE_GREEN] = CreateWindowEx(0, "BUTTON", "Tx 3", WS_CHILD | WS_VISIBLE,
	//	w -5 + 4*edit_size_y, h, 2 * edit_size_y, 2 * edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_TEXTURE_GREEN, m_Instance, NULL);

	//m_Hwnd[Hwnds::INSPECTOR_TEXTURE_BLUE] = CreateWindowEx(0, "BUTTON", "Tx 4", WS_CHILD | WS_VISIBLE,
	//	w -5 + 6*edit_size_y, h , 2 * edit_size_y, 2 * edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_TEXTURE_BLUE, m_Instance, NULL);

	m_Hwnd[Hwnds::INSPECTOR_TEXTURE] = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT |
		LVS_EDITLABELS, w - 5, h, 4*size_x, 170,
		m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_TEXTURE, m_Instance, NULL);

	ListView_SetExtendedListViewStyle(m_Hwnd[Hwnds::INSPECTOR_TEXTURE], LVS_EX_FULLROWSELECT);

	HIMAGELIST himl;
	HBITMAP hbmp;

	himl = ImageList_Create(32, 32, ILC_COLOR32, 10, 0);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/grass_ground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/grassFlowers.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/sand.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)(HBITMAP)LoadImage(m_Instance, "Data/Editor/G3_Architecture_Ground_City_03_Diffuse_01.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);
	ListView_SetImageList(m_Hwnd[Hwnds::INSPECTOR_TEXTURE], himl, LVSIL_SMALL);

	LVCOLUMN lvc;
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.cx = 4 * size_x;
	lvc.pszText = "Paint texture :";
	ListView_InsertColumn(m_Hwnd[Hwnds::INSPECTOR_TEXTURE], 0, &lvc);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;

	lvi.pszText = (LPSTR)("Background Texture");
	lvi.iItem = 0;
	lvi.iSubItem = 0;
	lvi.iImage = 0;
	ListView_InsertItem(m_Hwnd[Hwnds::INSPECTOR_TEXTURE], &lvi);

	lvi.pszText = (LPSTR)("Red Texture");
	lvi.iItem = 1;
	lvi.iSubItem = 0;
	lvi.iImage = 1;
	ListView_InsertItem(m_Hwnd[Hwnds::INSPECTOR_TEXTURE], &lvi);

	lvi.pszText = (LPSTR)("Green Texture");
	lvi.iItem = 2;
	lvi.iSubItem = 0;
	lvi.iImage = 2;
	ListView_InsertItem(m_Hwnd[Hwnds::INSPECTOR_TEXTURE], &lvi);

	lvi.pszText = (LPSTR)("Blue Texture");
	lvi.iItem = 3;
	lvi.iSubItem = 0;
	lvi.iImage = 3;
	ListView_InsertItem(m_Hwnd[Hwnds::INSPECTOR_TEXTURE], &lvi);

	h += 8*edit_size_y;
	m_Hwnd[Hwnds::INSPECTOR_PAINT_SIZE_TEXT] = CreateWindowEx(0, "STATIC", "Brush: ", WS_CHILD | WS_VISIBLE, w, h +2, 50, 20, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_PAINT_SIZE] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "2.0", WS_CHILD | WS_VISIBLE, w + 50, h, size_x + 25, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_PAINT_SIZE, m_Instance, NULL);
	
	h += edit_size_y;
	m_Hwnd[Hwnds::INSPECTOR_PAINT_STRENGTH_TEXT] = CreateWindowEx(0, "STATIC", "Strength: ", WS_CHILD | WS_VISIBLE, w, h +2, 50, 20, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_PAINT_STRENGTH] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "1.00", WS_CHILD | WS_VISIBLE, w + 50, h, size_x + 25, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_PAINT_STRENGTH, m_Instance, NULL);

	h += edit_size_y;
	m_Hwnd[Hwnds::INSPECTOR_TEXT_UPPER_LIMIT] = CreateWindowEx(0, "STATIC", "Up-limit: ", WS_CHILD | WS_VISIBLE, w, h + 2, 50, 20, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_UPPER_LIMIT] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "1.00", WS_CHILD | WS_VISIBLE, w + 50, h, size_x + 25, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_UPPER_LIMIT, m_Instance, NULL);

	m_Hwnd[Hwnds::INSPECTOR_HEIGHT_LIMIT] = CreateWindowEx(0, "BUTTON", "", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		w + size_x + 80, h, 50, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_HEIGHT_LIMIT, m_Instance, NULL);

	h += edit_size_y;
	m_Hwnd[Hwnds::INSPECTOR_TEXT_DOWN_LIMIT] = CreateWindowEx(0, "STATIC", "D-limit: ", WS_CHILD | WS_VISIBLE, w, h + 2, 50, 20, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_DOWN_LIMIT] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "1.00", WS_CHILD | WS_VISIBLE, w + 50, h, size_x + 25, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_DOWN_LIMIT, m_Instance, NULL);

}

void CSceneEditor::CreateTransformView(int &w, int &h, int edit_size_x, int edit_size_y)
{
	//Position
	m_Hwnd[Hwnds::INSPECTOR_TEXT_POSITION] = CreateWindowEx(0, "STATIC", "Positions", WS_CHILD | WS_VISIBLE, w, h, 150, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_X] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_POSITION_X, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Y] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_POSITION_Y, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Z] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + 2 * edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_POSITION_Z, m_Instance, NULL);

	h += 2 * edit_size_y;
	//Rotation
	m_Hwnd[Hwnds::INSPECTOR_TEXT_ROTATION] = CreateWindowEx(0, "STATIC", "Rotation", WS_CHILD | WS_VISIBLE, w, h, 150, 20, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_X] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_ROTATION_X, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Y] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_ROTATION_Y, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Z] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + 2 * edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_ROTATION_Z, m_Instance, NULL);

	//Scale
	h += 2 * edit_size_y;
	//Rotation
	m_Hwnd[Hwnds::INSPECTOR_TEXT_SCALE] = CreateWindowEx(0, "STATIC", "Scale", WS_CHILD | WS_VISIBLE, w, h, 150, 20, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_X] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_SCALE_X, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Y] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_SCALE_Y, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Z] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + 2 * edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_SCALE_Z, m_Instance, NULL);
}

void CSceneEditor::UpdateInspector()
{
	glm::vec3 position = m_CurrentEntity->GetLocalPosition();
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_X], position.x);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Y], position.y);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Z], position.z);

	glm::vec3 rotation = m_CurrentEntity->GetRotation();
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_X], rotation.x);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Y], rotation.y);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Z], rotation.z);

	glm::vec3 scale = m_CurrentEntity->GetScale();
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_X], scale.x);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Y], scale.y);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Z], scale.z);
}

void CSceneEditor::UpdateEntity(ControlsIds::Ids type)
{
	if (m_CurrentEntity == nullptr)
		return;

	switch (type)
	{
		case ControlsIds::INSPECTOR_EDIT_POSITION_X:
			{
			float x;
				GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_X], x);
				m_CurrentEntity->SetPositionX(x);
			}		
			break;
		case ControlsIds::INSPECTOR_EDIT_POSITION_Y:
		{
			float y;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Y], y);
			m_CurrentEntity->SetPositionY(y);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_POSITION_Z:
		{
			float z;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Z], z);
			m_CurrentEntity->SetPositionZ(z);
		}
		break;

		case ControlsIds::INSPECTOR_EDIT_ROTATION_X:
		{
			float x;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_X], x);
			m_CurrentEntity->SetRotationX(x);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_ROTATION_Y:
		{
			float y;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Y], y);
			m_CurrentEntity->SetRotationY(y);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_ROTATION_Z:
		{
			float z;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Z], z);
			m_CurrentEntity->SetRotationZ(z);
		}
		break;

		case ControlsIds::INSPECTOR_EDIT_SCALE_X:
		{
			float x;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_X], x);
			m_CurrentEntity->SetScaleX(x);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_SCALE_Y:
		{
			float y;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Y], y);
			m_CurrentEntity->SetScaleY(y);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_SCALE_Z:
		{
			float z;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Z], z);
			m_CurrentEntity->SetScaleZ(z);
		}
		break;

	}

}
void CSceneEditor::GetValueFromControl(HWND hwnd, float & value)
{
	
	std::string string_value = GetTextFromControl(hwnd);
	if (string_value.size() <= 0) return;
	else if (string_value.size() == 1 && string_value[0] == '.') return;

	if (string_value.find_first_not_of("1234567890.-") != string::npos) return;
	if (string_value.size() == 1)
		if (string_value[0] == '-' || string_value[0] == '.') return;
	value = stof(string_value);
}

void CSceneEditor::TexturePaintProcedure(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case ControlsIds::INSPECTOR_TEXTURE:
		//if (((LPNMHDR)lParam)->code == NM_CLICK)
		//{			
		//}
		if (((LPNMHDR)lParam)->code == NM_CLICK)
		{
			int index = ListView_GetSelectionMark(m_Hwnd[Hwnds::INSPECTOR_TEXTURE]);
			switch (index)
			{
			case 0:
				m_Game.GetCurrentScene()->m_PaintColor = glm::vec3(1 - m_PaintStrength, 1 - m_PaintStrength, 1 - m_PaintStrength);
				m_CurrentPaintColor = 0;
				break;
			case 1:				
				m_Game.GetCurrentScene()->m_PaintColor = glm::vec3(m_PaintStrength, 0, 0);
				m_CurrentPaintColor = 1;
				break;
			case 2:
				m_Game.GetCurrentScene()->m_PaintColor = glm::vec3(0, m_PaintStrength, 0);
				m_CurrentPaintColor = 2;
				break;
			case 3:
				m_Game.GetCurrentScene()->m_PaintColor = glm::vec3(0, 0, m_PaintStrength);
				m_CurrentPaintColor = 3;
				break;
			}		
		}
		break;
	}
}

void CSceneEditor::InspectorProcedure(WPARAM wParam, LPARAM lParam)
{
	int nr = ControlsIds::INSPECTOR_EDIT_SCALE_Z - ControlsIds::INSPECTOR_EDIT_POSITION_X;
	for (int x = 0; x < nr +1 ; x++)
	{
		if (wParam == ControlsIds::INSPECTOR_EDIT_POSITION_X + x && EN_CHANGE == lParam)
		{
			UpdateEntity((ControlsIds::Ids)(ControlsIds::INSPECTOR_EDIT_POSITION_X + x));
		}
	}
	if (wParam == ControlsIds::INSPECTOR_TEXT_ATTACHED_OFFSET && EN_CHANGE == lParam)
	{
		float ayo = 0;
		GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_TEXT_ATTACHED_OFFSET], ayo);
		if (m_CurrentEntity != nullptr)
			m_CurrentEntity->GetAttachYOffset() = ayo;
	}
	if (wParam == ControlsIds::INSPECTOR_DOWN_LIMIT && EN_CHANGE == lParam)
	{
		float ayo = 0;
		GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_DOWN_LIMIT], ayo);	
		m_Game.GetCurrentScene()->m_HeightDownLimit = ayo;
			
	}
	if (wParam == ControlsIds::INSPECTOR_UPPER_LIMIT && EN_CHANGE == lParam)
	{
		float ayo = 0;
		GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_UPPER_LIMIT], ayo);
		m_Game.GetCurrentScene()->m_HeightUpperLimit = ayo;
	}
	if (wParam == ControlsIds::INSPECTOR_PAINT_STRENGTH)
	{
		GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_PAINT_STRENGTH], m_PaintStrength);
		m_Game.GetCurrentScene()->m_HeightPaintStrength = m_PaintStrength;
	}
	if (wParam == ControlsIds::INSPECTOR_PAINT_SIZE)
	{
		float x;
		GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_PAINT_SIZE], x);
		m_Game.GetCurrentScene()->m_BrushSize = x;
	}
	switch (wParam)
	{
	case ControlsIds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT:
		{
			m_AttachToTerrainHeight = !m_AttachToTerrainHeight;

			if (m_AttachToTerrainHeight)
			{
				if(m_CurrentEntity != nullptr)
					m_CurrentEntity->SetPosition(m_Game.GetCurrentScene()->CreatePositionVector(m_CurrentEntity->GetPositionXZ(), m_CurrentEntity->GetAttachYOffset()));
				PostMessage(m_Hwnd[Hwnds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT], BM_SETCHECK, BST_CHECKED, 0);
			}
			else
				PostMessage(m_Hwnd[Hwnds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT], BM_SETCHECK, BST_UNCHECKED, 0);
		}
		break;
	case ControlsIds::INSPECTOR_HEIGHT_LIMIT:
	{
		m_Game.GetCurrentScene()->m_ApplyLimits = !m_Game.GetCurrentScene()->m_ApplyLimits;

		if(m_Game.GetCurrentScene()->m_ApplyLimits)
			PostMessage(m_Hwnd[Hwnds::INSPECTOR_HEIGHT_LIMIT], BM_SETCHECK, BST_CHECKED, 0);
		else
			PostMessage(m_Hwnd[Hwnds::INSPECTOR_HEIGHT_LIMIT], BM_SETCHECK, BST_UNCHECKED, 0);
	}
	break;
	case ControlsIds::INSPECTOR_DELETE_CURRENT_SELECTED:
		if (m_CurrentEntity == nullptr) break;
		m_Game.GetCurrentScene()->DeleteEntity(m_CurrentEntity);
		FillObjectsTree();
		break;
	case ControlsIds::INSPECTOR_GO_CAMERA_TO_OBJECT:
		{
			if (m_CurrentEntity == nullptr) break;
			glm::vec3 new_camera_pos = m_CurrentEntity->GetWorldPosition();
			float length = Utils::GetMaxFromVector(m_CurrentEntity->GetNormalizedSize());
			if (length <= 0) length = 5;
			
			new_camera_pos.z += 1.5f* length;
			new_camera_pos.y += 1.5f * length;
			m_Game.GetCurrentScene()->GetCamera()->SetPosition(new_camera_pos);
			m_Game.GetCurrentScene()->GetCamera()->SetPitch(45);
			m_Game.GetCurrentScene()->GetCamera()->SetYaw(0);
		}
		break;

	}
}
#endif