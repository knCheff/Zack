#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include "GameSettings.h"

class SettingsDialog : public wxDialog
{
public:
    SettingsDialog(wxWindow* parent, GameSettings* settings);

private:
    GameSettings* m_settings;
    wxSpinCtrl* m_gridSizeCtrl;
    wxSpinCtrl* m_intervalCtrl;
    wxColourPickerCtrl* m_livingColorCtrl;
    wxColourPickerCtrl* m_deadColorCtrl;
    wxCheckBox* m_showNeighborCountCtrl;

    void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};
#endif

