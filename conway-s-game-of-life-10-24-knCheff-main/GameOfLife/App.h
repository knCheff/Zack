#pragma once
#include "MainWindow.h"
#include "wx/wx.h"

class App : public wxApp
{
private:

public:
	App();
	~App();
	virtual bool OnInit();

private:
	MainWindow* m_mainWindow;
};

