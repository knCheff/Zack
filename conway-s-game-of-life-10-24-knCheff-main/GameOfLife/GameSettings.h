#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H


#include <wx/colour.h>
#include <fstream>
struct GameSettings
{
	bool showNeighborCount = false;
	//living cell colors
	unsigned int livingRed = 128;
	unsigned int livingGreen = 128;
	unsigned int livingBlue = 128;
	unsigned int livingAlpha = 255;

		// dead cell colors
	unsigned int deadRed = 255;
	unsigned int deadGreen = 255;
	unsigned int deadBlue = 255;
	unsigned int deadAlpha = 255;

		//grid and animation settings
	unsigned int gridSize = 15;
	unsigned int interval = 50;

	//color getters
	wxColour GetLivingColor() const
	{
		return wxColor(livingRed, livingGreen, livingBlue, livingAlpha);
	}
	wxColour GetDeadColor() const
	{
		return wxColor(deadRed, deadGreen, deadBlue, deadAlpha);
	}

	//color setters
	void SetLivingColor(const wxColour& color)
	{
		livingRed = color.GetRed();
		livingGreen = color.GetGreen();
		livingBlue = color.GetBlue();
		livingAlpha = color.GetAlpha();
	}

	void SetDeadColor(const wxColour& color)
	{
		deadRed = color.GetRed();
		deadGreen = color.GetGreen();
		deadBlue = color.GetBlue();
		deadAlpha = color.GetAlpha();
	}
	void LoadSettings()
	{
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		if (file.is_open())
		{
			file.read((char*)this, sizeof(GameSettings));
			file.close();
		}
	}

	void SaveSettings()
	{
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		if (file.is_open())
		{
			file.write((char*)this, sizeof(GameSettings));
			file.close();
		}
	}
};

#endif