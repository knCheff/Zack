
#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "GameSettings.h"
#include "SettingsDialog.h"
#include <wx/artprov.h>
#include <wx/numdlg.h>



wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::OnSizeChange) // Map the wxEVT_SIZE event to the handler
EVT_MENU(10001, MainWindow::OnPlayButton)
EVT_MENU(10002, MainWindow::OnPauseButton)
EVT_MENU(10003, MainWindow::OnNextButton)
EVT_MENU(10004, MainWindow::OnClearGridButton)
EVT_TIMER(10005, MainWindow::OnTimer)
EVT_TOOL(10006, MainWindow::OnOpenSettings)
EVT_MENU(10007, MainWindow::OnToggleShowNeighborCount)
EVT_MENU(10008, MainWindow::OnRandomize)
EVT_MENU(10009, MainWindow::OnRandomizeWithSeed)
EVT_MENU(wxID_NEW, MainWindow::OnNew)
EVT_MENU(wxID_OPEN, MainWindow::OnOpen)
EVT_MENU(wxID_SAVE, MainWindow::OnSave)
EVT_MENU(wxID_SAVEAS, MainWindow::OnSaveAs)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
wxEND_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(200, 200)), _generationCount(0), _livingCellsCount(0)
{
	CreateMenuBar();
	settings.LoadSettings();
	InitializeGrid();
	_drawingPanel = new DrawingPanel(this, _gameBoard);
	_drawingPanel->SetSettings(&settings);
	//Initialize status bar
	_statusBar = this->CreateStatusBar();
	UpdateStatusBar();

	_toolBar = CreateToolBar();
	//add play button
	wxBitmap playIcon(play_xpm);
	_toolBar->AddTool(10001, "Play", playIcon);

	//add play button
	wxBitmap pauseIcon(pause_xpm);
	_toolBar->AddTool(10002, "Pause", pauseIcon);

	//add play button
	wxBitmap nextIcon(next_xpm);
	_toolBar->AddTool(10003, "Next", nextIcon);

	//add play button
	wxBitmap trashIcon(trash_xpm);
	_toolBar->AddTool(10004, "Clear", trashIcon);


	_toolBar->Realize();

	//initialize timer
	_timer = new wxTimer(this, 10005);
	// Ensure layout includes the status bar
	this->Layout();
}
MainWindow::~MainWindow()
{
	if (_timer)
	{
		_timer->Stop();
		delete _timer;
	}
}

void MainWindow::OnSizeChange(wxSizeEvent& event)
{
	wxSize newSize = event.GetSize();
	if (_drawingPanel)
	{
		_drawingPanel->UpdateSize(newSize);
	}
	event.Skip(); 
}
void MainWindow::InitializeGrid()
{
	_gameBoard.resize(settings.gridSize);
	for (int i = 0; i < settings.gridSize; ++i)
	{
		_gameBoard[i].resize(settings.gridSize, false);
	}
}


void MainWindow::UpdateStatusBar()
{
	wxString statusText = wxString::Format("Generation: %d | Living Cells: %d", _generationCount, _livingCellsCount);
	_statusBar->SetStatusText(statusText);
}

void MainWindow::OnPlayButton(wxCommandEvent& play)
{
	_timer->Start(settings.interval);
}

void MainWindow::OnPauseButton(wxCommandEvent& pause)
{
	_timer->Stop();
}

void MainWindow::OnNextButton(wxCommandEvent& next)
{
	CreateNextGeneration();
}

void MainWindow::OnTimer(wxTimerEvent& timer)
{
	CreateNextGeneration();
	
}

void MainWindow::OnClearGridButton(wxCommandEvent& trash)
{
	for (int col = 0; col < settings.gridSize; col++)
	{
		for (int row = 0; row < settings.gridSize; row++)
		{
			_gameBoard[col][row] = false;
		}

		_generationCount = 0;
		_livingCellsCount = 0;
		UpdateStatusBar();
		_drawingPanel->Refresh();
	}
}

auto MainWindow::NeighborCount(int row, int col)
{
	int count = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0) continue;

			int newRow = row + i;
			int newCol = col + j;

			if (newRow >= 0 && newRow < settings.gridSize && newCol >= 0 && newCol < settings.gridSize)
			{
				if (_gameBoard[newRow][newCol])
				{
					count++;
				}
			}
		}
	}
	return count;
}

void MainWindow::CreateNextGeneration()
{
	std::vector<std::vector<bool>> sandbox(settings.gridSize, std::vector<bool>(settings.gridSize, false));
	_livingCellsCount = 0;
	bool hasChanged = false;  // Add flag to track changes

	for (int row = 0; row < settings.gridSize; ++row)
	{
		for (int col = 0; col < settings.gridSize; ++col)
		{
			int neighbor = NeighborCount(row, col);
			bool currentCell = _gameBoard[row][col];
			bool willLive = false;  // Track next state

			if (currentCell)
			{
				willLive = (neighbor == 2 || neighbor == 3);
				if (willLive) ++_livingCellsCount;
			}
			else
			{
				willLive = (neighbor == 3);
				if (willLive) ++_livingCellsCount;
			}

			sandbox[row][col] = willLive;

			// Check if this cell's state changed
			if (currentCell != willLive)
			{
				hasChanged = true;
			}
		}
		UpdateNeighborCounts();
		_drawingPanel->SetNeighborCounts(m_neighborCounts);
		_drawingPanel->Refresh();
	}

	// If no cells changed and timer is running, stop it
	if (!hasChanged && _timer->IsRunning())
	{
		_timer->Stop();
		wxMessageBox("Simulation has stabilized!", "Game of Life",
			wxOK | wxICON_INFORMATION);
	}

	_gameBoard.swap(sandbox);
	++_generationCount;
	UpdateStatusBar();
	_drawingPanel->Refresh();
}


void MainWindow::CreateMenuBar()
{
		wxMenuBar* menuBar = new wxMenuBar();

		// File menu
		wxMenu* fileMenu = new wxMenu();
		fileMenu->Append(wxID_NEW, "&New");
		fileMenu->Append(wxID_OPEN, "&Open");
		fileMenu->Append(wxID_SAVE, "&Save");
		fileMenu->Append(wxID_SAVEAS, "Save &As");
		fileMenu->AppendSeparator();
		fileMenu->Append(wxID_EXIT, "E&xit");
		menuBar->Append(fileMenu, "&File");

		// Options menu
		wxMenu* optionsMenu = new wxMenu();
		optionsMenu->Append(10006, "Settings");
		optionsMenu->Append(10008, "Randomize");
		optionsMenu->Append(10009, "Randomize with Seed");
		menuBar->Append(optionsMenu, "Options");

		// View menu
		wxMenu* viewMenu = new wxMenu();
		m_showNeighborCountMenuItem = new wxMenuItem(viewMenu, 10007, "Show Neighbor Count", "", wxITEM_CHECK);
		m_showNeighborCountMenuItem->Check(settings.showNeighborCount);
		viewMenu->Append(m_showNeighborCountMenuItem);
		menuBar->Append(viewMenu, "View");

		SetMenuBar(menuBar);
}

void MainWindow::OnOpenSettings(wxCommandEvent& event)
{
	SettingsDialog dialog(this, &settings);
	if (dialog.ShowModal() == wxID_OK)
	{
		InitializeGrid();
		_drawingPanel->Refresh();
	}
}
	
void MainWindow::OnToggleShowNeighborCount(wxCommandEvent& event)
{
	settings.showNeighborCount = m_showNeighborCountMenuItem->IsChecked();
	settings.SaveSettings();
	_drawingPanel->Refresh();
}

void MainWindow::UpdateNeighborCounts()
{
	m_neighborCounts.resize(settings.gridSize);
	for (int i = 0; i < settings.gridSize; ++i)
	{
		m_neighborCounts[i].resize(settings.gridSize);
		for (int j = 0; j < settings.gridSize; ++j)
		{
			m_neighborCounts[i][j] = NeighborCount(i, j);
		}
	}
}

	void MainWindow::OnRandomize(wxCommandEvent & event)
	{
		RandomizeGrid(time(NULL));
	}

	void MainWindow::OnRandomizeWithSeed(wxCommandEvent & event)
	{
		long seed = wxGetNumberFromUser("Enter a seed for randomization:",
			"Seed", "Randomize with Seed",
			0, 0, LONG_MAX);
		if (seed != -1) // -1 is returned if the user cancels
		{
			RandomizeGrid(seed);
		}
	}

	void MainWindow::RandomizeGrid(long seed)
	{
		srand(seed);

		int livingPercentage = rand() % 51 + 25; // Random percentage between 25% and 75%

		for (int i = 0; i < settings.gridSize; ++i)
		{
			for (int j = 0; j < settings.gridSize; ++j)
			{
				_gameBoard[i][j] = (rand() % 100 < livingPercentage);
			}
		}

		UpdateNeighborCounts();
		_drawingPanel->Refresh();
		UpdateStatusBar();
	}
	void MainWindow::OnNew(wxCommandEvent& event)
	{
		_timer->Stop();
		m_currentFile.Clear();
		OnClearGridButton(event);
	}

	void MainWindow::OnOpen(wxCommandEvent& event)
	{
		_timer->Stop();
		wxFileDialog openDialog(this, "Open Game of Life Pattern", "", "",
			"Cell files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

		if (openDialog.ShowModal() == wxID_CANCEL)
			return;

		if (LoadFromFile(openDialog.GetPath()))
		{
			m_currentFile = openDialog.GetPath();
		}
	}

	void MainWindow::OnSave(wxCommandEvent& event)
	{
		if (m_currentFile.IsEmpty())
		{
			OnSaveAs(event);
		}
		else
		{
			SaveToFile(m_currentFile);
		}
	}

	void MainWindow::OnSaveAs(wxCommandEvent& event)
	{
		wxFileDialog saveDialog(this, "Save Game of Life Pattern", "", "",
			"Cell files (*.cells)|*.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (saveDialog.ShowModal() == wxID_CANCEL)
			return;

		if (SaveToFile(saveDialog.GetPath()))
		{
			m_currentFile = saveDialog.GetPath();
		}
	}

	void MainWindow::OnExit(wxCommandEvent& event)
	{
		Close();
	}

	bool MainWindow::SaveToFile(const wxString& filename)
	{
		wxFile file;
		if (!file.Open(filename, wxFile::write))
		{
			wxMessageBox("Could not open file for writing!", "Error",
				wxOK | wxICON_ERROR);
			return false;
		}

		wxString content;
		content.Append("!Game of Life Pattern\n");

		for (size_t row = 0; row < settings.gridSize; ++row)
		{
			for (size_t col = 0; col < settings.gridSize; ++col)
			{
				content.Append(_gameBoard[row][col] ? '*' : '.');
			}
			content.Append('\n');
		}

		bool success = file.Write(content);
		file.Close();

		if (!success)
		{
			wxMessageBox("Error writing to file!", "Error",
				wxOK | wxICON_ERROR);
		}
		return success;
	}

	bool MainWindow::LoadFromFile(const wxString& filename)
	{
		wxTextFile file(filename);
		if (!file.Open())
		{
			wxMessageBox("Could not open file!", "Error",
				wxOK | wxICON_ERROR);
			return false;
		}

		// First pass: determine grid size
		size_t maxWidth = 0;
		size_t height = 0;

		for (wxString line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine())
		{
			if (!line.StartsWith('!'))  // Skip comments
			{
				maxWidth = wxMax(maxWidth, line.Length());
				++height;
			}
		}

		// Resize grid to match file contents
		size_t newSize = wxMax(maxWidth, height);
		ResizeGrid(newSize);

		// Second pass: load the pattern
		size_t row = 0;
		file.GoToLine(0);

		for (wxString line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine())
		{
			if (!line.StartsWith('!'))
			{
				for (size_t col = 0; col < line.Length(); ++col)
				{
					_gameBoard[row][col] = (line[col] == '*');
				}
				++row;
			}
		}

		file.Close();
		_drawingPanel->Refresh();
		return true;
	}

	void MainWindow::ResizeGrid(size_t newSize)
	{
		settings.gridSize = newSize;
		_gameBoard.resize(newSize);
		for (auto& row : _gameBoard)
		{
			row.resize(newSize, false);
		}
		_drawingPanel->Refresh();
	}
	