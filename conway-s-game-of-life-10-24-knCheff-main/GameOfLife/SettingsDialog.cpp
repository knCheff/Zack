#include "SettingsDialog.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
EVT_BUTTON(wxID_OK, SettingsDialog::OnOK)
EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent, GameSettings* settings)
    : wxDialog(parent, wxID_ANY, "Game Settings"), m_settings(settings)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    // Grid Size
    wxBoxSizer* gridSizeSizer = new wxBoxSizer(wxHORIZONTAL);
    gridSizeSizer->Add(new wxStaticText(this, wxID_ANY, "Grid Size:"), 0, wxALL, 5);
    m_gridSizeCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 5, 100, m_settings->gridSize);
    gridSizeSizer->Add(m_gridSizeCtrl, 0, wxALL, 5);
    mainSizer->Add(gridSizeSizer, 0, wxEXPAND | wxALL, 5);

    // Interval
    wxBoxSizer* intervalSizer = new wxBoxSizer(wxHORIZONTAL);
    intervalSizer->Add(new wxStaticText(this, wxID_ANY, "Interval (ms):"), 0, wxALL, 5);
    m_intervalCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1000, m_settings->interval);
    intervalSizer->Add(m_intervalCtrl, 0, wxALL, 5);
    mainSizer->Add(intervalSizer, 0, wxEXPAND | wxALL, 5);

    // Living Cell Color
    wxBoxSizer* livingColorSizer = new wxBoxSizer(wxHORIZONTAL);
    livingColorSizer->Add(new wxStaticText(this, wxID_ANY, "Living Cell Color:"), 0, wxALL, 5);
    m_livingColorCtrl = new wxColourPickerCtrl(this, wxID_ANY, m_settings->GetLivingColor());
    livingColorSizer->Add(m_livingColorCtrl, 0, wxALL, 5);
    mainSizer->Add(livingColorSizer, 0, wxEXPAND | wxALL, 5);

    // Dead Cell Color
    wxBoxSizer* deadColorSizer = new wxBoxSizer(wxHORIZONTAL);
    deadColorSizer->Add(new wxStaticText(this, wxID_ANY, "Dead Cell Color:"), 0, wxALL, 5);
    m_deadColorCtrl = new wxColourPickerCtrl(this, wxID_ANY, m_settings->GetDeadColor());
    deadColorSizer->Add(m_deadColorCtrl, 0, wxALL, 5);
    mainSizer->Add(deadColorSizer, 0, wxEXPAND | wxALL, 5);

    // Show Neighbor Count
    wxBoxSizer* neighborCountSizer = new wxBoxSizer(wxHORIZONTAL);
    neighborCountSizer->Add(new wxStaticText(this, wxID_ANY, "Show Neighbor Count:"), 0, wxALL, 5);
    m_showNeighborCountCtrl = new wxCheckBox(this, wxID_ANY, "");
    m_showNeighborCountCtrl->SetValue(m_settings->showNeighborCount);
    neighborCountSizer->Add(m_showNeighborCountCtrl, 0, wxALL, 5);
    mainSizer->Add(neighborCountSizer, 0, wxEXPAND | wxALL, 5);

    // OK and Cancel buttons
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, 10);

    mainSizer->Fit(this);
}

void SettingsDialog::OnOK(wxCommandEvent& event)
{
    m_settings->gridSize = m_gridSizeCtrl->GetValue();
    m_settings->interval = m_intervalCtrl->GetValue();
    m_settings->SetLivingColor(m_livingColorCtrl->GetColour());
    m_settings->SetDeadColor(m_deadColorCtrl->GetColour());
    m_settings->showNeighborCount = m_showNeighborCountCtrl->GetValue();
    m_settings->SaveSettings();
    EndModal(wxID_OK);
}

void SettingsDialog::OnCancel(wxCommandEvent& event)
{
    m_settings->LoadSettings();
    EndModal(wxID_CANCEL);
}
