#include "App.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

bool App::OnInit() 
{
	m_mainWindow = new MainWindow();
	m_mainWindow->Show();

	return true;
}