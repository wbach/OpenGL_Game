#pragma once
namespace Hwnds
{
	enum Type
	{
		MAIN_WINDOW = 0,
		OPENGL_WINDOW,
		//Inscpector
		INSPECTOR_TEXT_POSITION,
		INSPECTOR_TEXT_ROTATION,
		INSPECTOR_TEXT_SCALE,
		INSPECTOR_EDIT_POSITION_X,
		INSPECTOR_EDIT_POSITION_Y,
		INSPECTOR_EDIT_POSITION_Z,
		INSPECTOR_EDIT_ROTATION_X,
		INSPECTOR_EDIT_ROTATION_Y,
		INSPECTOR_EDIT_ROTATION_Z,
		INSPECTOR_EDIT_SCALE_X,
		INSPECTOR_EDIT_SCALE_Y,
		INSPECTOR_EDIT_SCALE_Z,
		INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT,
		INSPECTOR_DELETE_CURRENT_SELECTED,
		INSPECTOR_TEXT_CURRENT_SELECTED,
		INSPECTOR_GO_CAMERA_TO_OBJECT,
		INSPECTOR_TEXTURE,
		INSPECTOR_PAINT_SIZE,
		INSPECTOR_PAINT_STRENGTH,
		INSPECTOR_PAINT_SIZE_TEXT,
		INSPECTOR_PAINT_STRENGTH_TEXT,
		INSPECTOR_TEXT_ATTACHED_OFFSET,
		INSPECTOR_TEXT_ATTACHED_OFFSET_TEXT,
		INSPECTOR_TEXT_UPPER_LIMIT,
		INSPECTOR_TEXT_DOWN_LIMIT,
		INSPECTOR_UPPER_LIMIT,
		INSPECTOR_DOWN_LIMIT,
		INSPECTOR_HEIGHT_LIMIT,
		//File list
		FILE_LIST,
		DRIVE_COMBO,
		CURRENT_PATH,
		SPAWN_ENITY_DIALOG,
		SPAWN_ENITY_DIALOG_COMBO,
		SPAWN_ENITY_DIALOG_PARENT_TEXT,
		SPAWN_ENITY_DIALOG_CURRENT_FILE,
		SPAWN_ENITY_DIALOG_NORMALIZED_MESH,
		SPAWN_ENITY_DIALOG_EDIT_X,
		SPAWN_ENITY_DIALOG_EDIT_Y,
		SPAWN_ENITY_DIALOG_EDIT_Z,
		SPAWN_ENITY_DIALOG_BUTTON_OK,
		SPAWN_ENITY_DIALOG_BUTTON_CANCEL,
		//Tree
		OBJECT_TREEE,
		//Menu
		MENU,
		MENU_BAR,
		//
		LOADING_DIALOG,
		LOADING_PROGRESS_BAR,
		//GUIEditor
		GUI_MAIN_DIALOG,
		GUI_TREE_VIEW,
		//COunt
		COUNT
	};
}