#include "property_editor.h"
#include "property_editor_win32.h"
#include "widget_helper_win32.h"
#include "propertygrid.h"

using namespace gui_imp;
using namespace gui;

void property_editor_impl_win32::make(const property_editor_initializer &a_Initializer){
	widget_helper_win32::load_common_controls();
	
	HWND l_ParentWnd = (HWND)a_Initializer.get_parent()->handle();

	{
		//widget_hook_win32(this);

		m_hWnd = New_PropertyGrid(l_ParentWnd, a_Initializer.get_top(), a_Initializer.get_left(), 
			a_Initializer.get_width(), a_Initializer.get_height());

		PROPGRIDITEM Item;
		PropGrid_ItemInit(Item);

		SYSTEMTIME stProp13;

		GetLocalTime(&stProp13);

		PROPGRIDFDITEM ItemFd = {0};
		ItemFd.lpszDlgTitle = TEXT("Choose File");
		ItemFd.lpszFilePath = TEXT("");
		ItemFd.lpszFilter = TEXT("Text files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
		ItemFd.lpszDefExt = TEXT("txt");

		Item.lpszCatalog = TEXT("Edit, Static, and Combos");
		Item.lpszPropName = TEXT("Date and Time Field");
		Item.lpCurValue = (LPARAM)&ItemFd;
		Item.lpszPropDesc = TEXT("Pick a time.");
		Item.iItemType = PIT_FILE;
		PropGrid_AddItem(m_hWnd, &Item);

		PropGrid_ShowToolTips(m_hWnd, TRUE);
		PropGrid_ExpandAllCatalogs(m_hWnd);
		PropGrid_SetCurSel(m_hWnd, 0);
		PropGrid_ShowPropertyDescriptions(m_hWnd, TRUE);

		InvalidateRect(l_ParentWnd, NULL, TRUE);
	}
}

void property_editor_impl_win32::clear()
{
	PropGrid_ResetContent(m_hWnd);
}

size_t property_editor_impl_win32::add_item(const property_item &)
{
	//

	return 0;
}

void property_editor_impl_win32::remove_item(size_t idx)
{
	PropGrid_DeleteItem(m_hWnd, idx);
}

size_t property_editor_impl_win32::get_item_count()
{
	return (size_t)PropGrid_GetCount(m_hWnd);
}

size_t property_editor_impl_win32::get_selected_item()
{
	return (size_t)PropGrid_GetCurSel(m_hWnd);
}

void property_editor_impl_win32::set_selected_item(size_t idx)
{
}
