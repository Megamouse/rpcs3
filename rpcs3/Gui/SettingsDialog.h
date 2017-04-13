#pragma once

class SettingsDialog : public wxDialog
{
public:
	SettingsDialog(wxWindow* parent, const std::string& path);

private:
	wxTextCtrl* s_module_search_box;
	wxCheckListBox* chbox_list_core_lle;
	wxRadioButton* rbut_all_core_lle;
	wxRadioButton* rbut_none_core_lle;
	wxRadioBox* rbox_lib_loader;

	void OnModuleListItemToggled(wxCommandEvent& event);
	void OnSearchBoxTextChanged(wxCommandEvent& event);
	void OnModuleListAllToggled(wxCommandEvent& event);
	void OnModuleListNoneToggled(wxCommandEvent& event);
	void OnLibLoaderModeToggled(wxCommandEvent& event);
	void EnableModuleList(bool enabled);
	void ToggleRadioButton(wxRadioButton* button, bool isChecked, bool needs_check);
	std::map<std::string, bool> lle_module_list;
};
