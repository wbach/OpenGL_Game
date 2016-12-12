#include "SceneEditor.h"

#ifdef EDITOR
void CSceneEditor::CreateFileList()
{
	RECT rcl;
	GetClientRect(m_Hwnd[Hwnds::MAIN_WINDOW], &rcl);

	m_Hwnd[Hwnds::DRIVE_COMBO] = CreateWindowEx(0, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, 0, 455, 50, 200, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::DRIVE_COMBO, m_Instance, NULL);

	m_Hwnd[Hwnds::CURRENT_PATH] = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE , 50, 458, m_Width / 2 - 375, 20,
		m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	SetWindowText(m_Hwnd[Hwnds::CURRENT_PATH], m_CurrentPath.c_str());

	m_Hwnd[Hwnds::FILE_LIST] = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT |
		LVS_EDITLABELS, 0, 485, rcl.right - rcl.left, 250,
		m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::FILE_LIST, m_Instance, NULL);

	ListView_SetExtendedListViewStyle(m_Hwnd[Hwnds::FILE_LIST], LVS_EX_FULLROWSELECT);

	
	ListView_SetTextBkColor(m_Hwnd[Hwnds::FILE_LIST], BKCOLOR);
	ListView_SetTextColor(m_Hwnd[Hwnds::FILE_LIST], TEXTCOLOR);
	ListView_SetOutlineColor(m_Hwnd[Hwnds::FILE_LIST], BKCOLOR);
	ListView_SetBkColor(m_Hwnd[Hwnds::FILE_LIST], BKCOLOR);

	HIMAGELIST himl;
	HBITMAP hbmp;

	himl = ImageList_Create(32, 32, ILC_COLOR32, 10, 0);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/file.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/folder.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(m_Instance, "Data/Editor/fbx.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)(HBITMAP)LoadImage(m_Instance, "Data/Editor/obj-icon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)(HBITMAP)LoadImage(m_Instance, "Data/Editor/omg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);
	ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);
	ListView_SetImageList(m_Hwnd[Hwnds::FILE_LIST], himl, LVSIL_SMALL);

	//ListView_SetGroupInfo(

	LVCOLUMN lvc;
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.cx = 300;
	lvc.pszText = "File name";
	ListView_InsertColumn(m_Hwnd[Hwnds::FILE_LIST], 0, &lvc);

	lvc.iSubItem = 1;
	lvc.cx = 50;
	lvc.pszText = "Size";
	ListView_InsertColumn(m_Hwnd[Hwnds::FILE_LIST], 1, &lvc);

	lvc.iSubItem = 2;
	lvc.cx = rcl.right - rcl.left - 350;
	lvc.pszText = "Date";
	ListView_InsertColumn(m_Hwnd[Hwnds::FILE_LIST], 2, &lvc);

	RegisterSpawEntityDialogClass();
//	CreateSpawEntityDialog();
}
void CSceneEditor::FillFileList()
{
	ListView_DeleteAllItems(m_Hwnd[Hwnds::FILE_LIST]);

	int i = 0;
	for (const SFile& file : m_CurrentPathFiles)
	{
		AddFile(i, file);
		i++;
	}
}
void CSceneEditor::AddFile(int i, const SFile& file)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;

	lvi.pszText = (LPSTR)(file.name.c_str());
	lvi.iItem = i;
	lvi.iSubItem = 0;

	std::string size = " ";

	if (file.fileType == FilesTypes::DIRECTORY)
	{
		size = "Directory";
		lvi.iImage = 1;
	}
	else if (file.fileType == FilesTypes::DEFAULT)
	{
		size = std::to_string(file.size) + " MB";
		std::string ext = file.name.substr(file.name.find_last_of('.') + 1);

		if (ext.size() <= 0)
		{
			lvi.mask = LVIF_TEXT;
		}
		else if (ext.compare("obj") == 0)
			lvi.iImage = 3;
		else if (ext.compare("fbx") == 0)
			lvi.iImage = 2;
		else if (ext.compare("jpg") == 0 || ext.compare("tga") == 0 || ext.compare("png") == 0 || ext.compare("bmp") == 0)
			lvi.iImage = 4;
		else
			lvi.iImage = 0;
	}
	//lvi.iImage = 1;
	ListView_InsertItem(m_Hwnd[Hwnds::FILE_LIST], &lvi);
	ListView_SetItemText(m_Hwnd[Hwnds::FILE_LIST], i, 1, (LPSTR) (size.c_str()));
	//ListView_SetItemText(m_Hwnd[Hwnds::FILE_LIST], i, 2, (LPSTR) file.date.c_str());

}
void CSceneEditor::FindFilesInDirectory(std::string path)
{
	SetWindowText(m_Hwnd[Hwnds::CURRENT_PATH], path.c_str());
	m_CurrentPathFiles.clear();

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	TCHAR szDir[MAX_PATH];

	StringCchCopy(szDir, MAX_PATH, path.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
		return;

	do
	{
		SFile file;
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			continue;

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			file.fileType = FilesTypes::DIRECTORY;
			file.name = ffd.cFileName;
		}
		else
		{
			file.fileType = FilesTypes::DEFAULT;
			file.name = ffd.cFileName;
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			file.size = static_cast<int>(filesize.QuadPart /(LONGLONG)1000000);
			FILETIME  lpCreationTime, lpLastAccessTime, lpLastWriteTime;
			GetFileTime(hFind, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime);
			SYSTEMTIME time;
			FileTimeToSystemTime(&lpCreationTime, &time);

			file.date = std::to_string(time.wDay) + "." + std::to_string(time.wMonth) + "." + std::to_string(time.wYear);
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
		m_CurrentPathFiles.push_back(file);
	} while (FindNextFile(hFind, &ffd) != 0);
	FillFileList();
}
void CSceneEditor::CheckAvaibleDrives()
{
	const WORD buff_size = 40;
	LPSTR buff = (LPSTR)GlobalAlloc(GPTR, buff_size);
	WORD required;

	required = (WORD)GetLogicalDriveStrings(buff_size, buff);
	if (required > buff_size) 
		return;
	
	std::string drive;
	for (int x = 0; x < required; x++)
		if (buff[x] != 0)
		{
			drive += buff[x];
		}
		else
		{
			SendMessage(m_Hwnd[Hwnds::DRIVE_COMBO], CB_ADDSTRING, 0, (LPARAM)drive.c_str());
			m_Drives.push_back(drive);
			drive.clear();
		}
	int i = 0;
	for (std::string& drive : m_Drives)
	{
		std::string d = m_CurrentPath.substr(0, m_CurrentPath.find_first_of('\\')) + '\\';
		if (drive.compare(d) == 0)
		{
			SendMessage(m_Hwnd[Hwnds::DRIVE_COMBO], CB_SETCURSEL, 0, (LPARAM)i);
			break;
		}
		i++;
	}
	GlobalFree(buff);
}
void CSceneEditor::SpawnEntity(std::string file_name, glm::vec3 normalized_scale)
{
	glm::vec3 position = m_Game.GetCurrentScene()->GetCameraPosition();
	glm::vec3 dir;
	float pitch = Utils::ToRadians(m_Game.GetCurrentScene()->GetCamera()->GetPitch());
	float yaw = Utils::ToRadians(m_Game.GetCurrentScene()->GetCamera()->GetYaw());
	float xzLen = cos(pitch);
	dir.z = xzLen * cos(yaw);
	dir.y = sin(pitch);
	dir.x = xzLen * sin(-yaw);
	dir = glm::normalize(dir) *-1.f;
	glm::vec3 spawn_position = position + 20.f*dir;

	if (m_AttachToTerrainHeight)
	{
		spawn_position = m_Game.GetCurrentScene()->CreatePositionVector(spawn_position.x, spawn_position.z, 0);
	}

	shared_ptr<CEntity> new_entity = m_Game.GetCurrentScene()->CreateEntityFromFile(file_name, ColiderType::NONE_COLIDER, glm::vec3(0), false, spawn_position);

	glm::mat4 nm = m_Game.GetCurrentScene()->GetLoader().m_Models[new_entity->GetModelId()]->CalculateNormalizedMatrix(normalized_scale.x, normalized_scale.y, normalized_scale.z);
	new_entity->SetNormalizedMatrix(nm);
	new_entity->SetNormalizedSize(normalized_scale);
	if (m_CurrentEntity != nullptr) 
	{
		m_CurrentEntity->AddSubbEntity(new_entity);
	}
	else
	{
		m_Game.GetCurrentScene()->AddEntity(new_entity);
	}

	m_CurrentEntity = new_entity;	
	FillObjectsTree();
}

void CSceneEditor::FileExplorerProcedure(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case ControlsIds::FILE_LIST:
		//if (((LPNMHDR)lParam)->code == NM_CLICK)
		//{			
		//}
		if (((LPNMHDR)lParam)->code == NM_DBLCLK)
		{
			int index = ListView_GetSelectionMark(m_Hwnd[Hwnds::FILE_LIST]);
			std::string file_all_path;

			int compare_result = m_CurrentPathFiles[index].name.compare("..");
			if (compare_result == 0)
			{
				m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.find_last_of('\\'));
				m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.find_last_of('\\'));
				m_CurrentPath += "\\";
				FindFilesInDirectory(m_CurrentPath);
				break;
			}
			else
			{
				file_all_path = m_CurrentPath + m_CurrentPathFiles[index].name;
			}
			//
			if (m_CurrentPathFiles[index].fileType == FilesTypes::DIRECTORY)
			{
				m_CurrentPath += m_CurrentPathFiles[index].name + "\\";
				FindFilesInDirectory(m_CurrentPath);
			}
			else if (m_CurrentPathFiles[index].fileType == FilesTypes::DEFAULT && compare_result != 0)
			{
				std::string file_extension = m_CurrentPathFiles[index].name.substr(m_CurrentPathFiles[index].name.find_last_of('.') + 1);
				if (!file_extension.compare("obj") || !file_extension.compare("fbx") || !file_extension.compare("3ds"))
				{
					m_SpawnedPathFile = file_all_path;
					CreateSpawEntityDialog();
				}
				else if (!file_extension.compare("map"))
				{
					m_Game.m_SceneParser.LoadScene(file_all_path, m_Game.GetCurrentScene(), [](int p) 
					{
						SendMessage(s_ProgresBarLoading, PBM_SETPOS, (WPARAM)p, 0);
					});
					FillObjectsTree();
				}
				else
				{
					ShellExecute(NULL, "open", file_all_path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
				}
			}
		}
		break;
	}
}

void CSceneEditor::CreateSpawEntityDialog()
{
	RECT rect;
	GetClientRect(GetDesktopWindow(), &rect);
	rect.left = (rect.right / 2) - (400 / 2);
	rect.top = (rect.bottom / 2) - (100 / 2);

	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG] = CreateWindowEx(WS_EX_TOPMOST, "SpawEntityDialogClass", "Spawn Entity",
		WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 270, 250, NULL, NULL, m_Instance, NULL);

	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_PARENT_TEXT] = CreateWindowEx(0, "STATIC", "Parent ", WS_CHILD | WS_VISIBLE
		, 10, 8, 35, 30, m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], NULL, m_Instance, NULL);	

	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_COMBO] = CreateWindowEx(0, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		CBS_DROPDOWN, 50, 5, 180, 200, m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], (HMENU)ControlsIds::SPAWN_ENITY_DIALOG_COMBO, m_Instance, NULL);

	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_CURRENT_FILE] = CreateWindowEx(0, "STATIC", "File : ", WS_CHILD | WS_VISIBLE
		, 10, 38, 250, 20, m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], NULL, m_Instance, NULL);
	
	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_NORMALIZED_MESH] = CreateWindowEx(0, "BUTTON", "Normalized mesh", WS_CHILD | WS_VISIBLE | BS_CHECKBOX
		, 50, 58, 200, 30, m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], (HMENU)ControlsIds::SPAWN_ENITY_DIALOG_NORMALIZED_MESH, m_Instance, NULL);
	PostMessage(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_NORMALIZED_MESH], BM_SETCHECK, BST_CHECKED, 0);
	m_NormalizeSpawnedMesh = true;

	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_EDIT_X] = CreateWindowEx(0, "EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER,
		50, 88, 150, 20, m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], (HMENU)ControlsIds::SPAWN_ENITY_DIALOG_EDIT_X, m_Instance, NULL);

	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_EDIT_Y] = CreateWindowEx(0, "EDIT", "1", WS_CHILD | WS_VISIBLE | WS_BORDER,
		50, 108, 150, 20, m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], (HMENU)ControlsIds::SPAWN_ENITY_DIALOG_EDIT_Y, m_Instance, NULL);

	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_EDIT_Z] = CreateWindowEx(0, "EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER,
		50, 128, 150, 20, m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], (HMENU)ControlsIds::SPAWN_ENITY_DIALOG_EDIT_Z, m_Instance, NULL);

	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_BUTTON_OK] = CreateWindowEx(0, "BUTTON", "Ok", WS_CHILD | WS_VISIBLE,
		25, 158, 100, 30, m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], (HMENU)ControlsIds::SPAWN_ENITY_DIALOG_BUTTON_OK, m_Instance, NULL);

	m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_BUTTON_CANCEL] = CreateWindowEx(0, "BUTTON", "Cancel", WS_CHILD | WS_VISIBLE,
		125, 158, 100, 30, m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], (HMENU)ControlsIds::SPAWN_ENITY_DIALOG_BUTTON_CANCEL, m_Instance, NULL);
	SetWindowLongPtr(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], GWLP_USERDATA, (long)this);

	SetFont();

	//ShowWindow(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG], SW_SHOWDEFAULT);
	//UpdateWindow(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG]);
}
void CSceneEditor::RegisterSpawEntityDialogClass()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = SpawnEntityDialogProc;
	wc.hInstance = m_Instance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszClassName = "SpawEntityDialogClass";
	RegisterClassEx(&wc);
}
LRESULT CSceneEditor::RealSpawnEntityDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SetWindowText(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_CURRENT_FILE], m_SpawnedPathFile.c_str());
	switch (msg)
	{
	case WM_ACTIVATE:
		{
			
		}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{			
		case ControlsIds::SPAWN_ENITY_DIALOG_COMBO:
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				SendMessage(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_COMBO], CB_RESETCONTENT, 0, 0);
				SendMessage(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_COMBO], CB_ADDSTRING, 0, (LPARAM) "None");				
				if (m_CurrentEntity != nullptr) 
				{
					SendMessage(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_COMBO], CB_ADDSTRING, 0, (LPARAM)m_CurrentEntity->GetNameWithID().c_str());
					SendMessage(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_COMBO], CB_SETCURSEL, 1, 0);
				}
				break;

			}
			break;
		case ControlsIds::SPAWN_ENITY_DIALOG_NORMALIZED_MESH:
			m_NormalizeSpawnedMesh = !m_NormalizeSpawnedMesh;

			if (m_NormalizeSpawnedMesh)
				PostMessage(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_NORMALIZED_MESH], BM_SETCHECK, BST_CHECKED, 0);
			else
				PostMessage(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_NORMALIZED_MESH], BM_SETCHECK, BST_UNCHECKED, 0);
			break;

		case ControlsIds::SPAWN_ENITY_DIALOG_BUTTON_OK:
		{
			float x = 0, y = 0, z = 0;
			if (m_NormalizeSpawnedMesh)
			{
				GetValueFromControl(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_EDIT_X], x);
				GetValueFromControl(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_EDIT_Y], y);
				GetValueFromControl(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_EDIT_Z], z);
			}
			if (SendMessage(m_Hwnd[Hwnds::SPAWN_ENITY_DIALOG_COMBO], CB_GETCURSEL, 0, 0) == 0)
				m_CurrentEntity = nullptr;

			SpawnEntity(m_SpawnedPathFile, glm::vec3(x, y, z));
			DestroyWindow(hwnd);
			break;
		}
		case ControlsIds::SPAWN_ENITY_DIALOG_BUTTON_CANCEL:
			DestroyWindow(hwnd);
			break;
			//MessageBox(hwnd, "Wcisn¹³eœ przycisk 1", "Test", MB_ICONINFORMATION);
			break;
		}
		break;
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

LRESULT CSceneEditor::SpawnEntityDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CSceneEditor* me = (CSceneEditor*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (me) return me->RealSpawnEntityDialogProc(hwnd, msg, wParam, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif