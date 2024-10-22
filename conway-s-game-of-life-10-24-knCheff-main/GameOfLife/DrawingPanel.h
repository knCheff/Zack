#pragma once
#include "wx/wx.h"
#include <vector>
#include "GameSettings.h"

class DrawingPanel : public wxPanel
{
public:
	//constructor
	DrawingPanel(wxFrame* parent, std::vector<std::vector<bool>>& gameBoard);
	//destructor
	~DrawingPanel();

	void OnMouseUp(wxMouseEvent& _leftClick);
	void OnPaint(wxPaintEvent& _paint);
	void UpdateSize(const wxSize& _size);
	void SetNeighborCounts(const std::vector<std::vector<int>>& neighborCounts);
	void SetSettings(GameSettings* settings);


private:
	int gridSize;
	std::vector<std::vector<int>> m_neighborCounts;
	std::vector<std::vector<bool>>& _gameBoard;
	GameSettings* _settings;

	wxDECLARE_EVENT_TABLE();
};

