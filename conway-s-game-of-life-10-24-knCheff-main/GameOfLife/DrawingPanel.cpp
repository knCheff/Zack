#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)// Map the wxEVT event to the handler
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxFrame* parent, std::vector<std::vector<bool>>& gameBoard) : wxPanel(parent, wxID_ANY), _gameBoard(gameBoard)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
    this->Bind(wxEVT_LEFT_UP, &DrawingPanel::OnMouseUp, this);
}
DrawingPanel::~DrawingPanel()
{

}

void DrawingPanel::OnPaint(wxPaintEvent& paint)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* context = wxGraphicsContext::Create(dc);
    if (!context) {
        return;
    }

    //get size of window
    wxSize windowSize = GetSize();

    //cell width and height
    int gridSize = _gameBoard.size();
    int cellWidth = windowSize.GetWidth() / gridSize;
    int cellHeight = windowSize.GetHeight() / gridSize;

    // loop for row equal to gridsize 
    for (int row = 0; row < gridSize; row++)
    {
        // loop for column equal to grid size "15"
        for (int col = 0; col < gridSize; col++)
        {

            int x = col * cellWidth;
            int y = row * cellHeight;

            if (_gameBoard[row][col])
            {
                context->SetBrush(_settings->GetLivingColor());
            }
            else
            {
                context->SetBrush(_settings->GetDeadColor());
            }
            context->SetPen(*wxBLACK_PEN);
            // drawing rectangle with width and height equal to cel size
            context->DrawRectangle(x, y, cellWidth, cellHeight);
        }
    }

    delete context;
}

void DrawingPanel::OnMouseUp(wxMouseEvent& leftClick)
{
    wxSize windowSize = GetSize();
    int gridSize = _gameBoard.size();

    int cellWidth = windowSize.GetWidth() / gridSize;
    int cellHeight = windowSize.GetHeight() / gridSize;

    int col = leftClick.GetX() / cellWidth;
    int row = leftClick.GetY() / cellHeight;

    if (col >= 0 && col < gridSize && row >= 0 &&  row < gridSize)
    {
        _gameBoard[row][col] = !_gameBoard[row][col];
        Refresh();
    }
}

void DrawingPanel::UpdateSize(const wxSize& size)
{
    SetSize(size);
    Refresh();
}
void DrawingPanel::SetSettings(GameSettings* settings)
{
    _settings = settings;
}
void DrawingPanel::SetNeighborCounts(const std::vector<std::vector<int>>& neighborCounts)
{
    m_neighborCounts = neighborCounts;
    Refresh(); // Trigger a repaint
}
