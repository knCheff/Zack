#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include <vector>
#include "GameSettings.h"
#include <wx/file.h>  
#include <wx/textfile.h>

class MainWindow : public wxFrame
{
public:
	MainWindow();
	~MainWindow();
	

private:
	void OnSizeChange(wxSizeEvent& event);
	void InitializeGrid();
	void UpdateStatusBar();
	void OnPlayButton(wxCommandEvent& play);
	void OnPauseButton(wxCommandEvent& pause);
	void OnNextButton(wxCommandEvent& next);
	void OnClearGridButton(wxCommandEvent& trash);
	void OnTimer(wxTimerEvent& timer);
	auto NeighborCount(int row, int col);
	void CreateNextGeneration();
	void CreateMenuBar();
	void OnOpenSettings(wxCommandEvent& event);
	void OnToggleShowNeighborCount(wxCommandEvent& event);
	void UpdateNeighborCounts();
	void OnRandomize(wxCommandEvent& event);
	void OnRandomizeWithSeed(wxCommandEvent& event);
	void RandomizeGrid(long seed);
	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	bool SaveToFile(const wxString& filename);
	bool LoadFromFile(const wxString& filename);
	void ResizeGrid(size_t newSize);

	std::vector<std::vector<bool>> _gameBoard;
	int _generationCount;
	int _livingCellsCount;
	wxMenuItem* m_showNeighborCountMenuItem;
	std::vector<std::vector<int>> m_neighborCounts;


	GameSettings settings;
	//drawingPanel pointer
	DrawingPanel* _drawingPanel;
	// status bar pointer
	wxStatusBar* _statusBar;
	// toolbar pointer
	wxToolBar* _toolBar;
	//timer pointer
	wxTimer* _timer;

	wxString m_currentFile;
	//declare the event table
	wxDECLARE_EVENT_TABLE(); 
};

