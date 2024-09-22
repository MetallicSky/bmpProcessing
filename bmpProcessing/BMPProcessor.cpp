#include "BMPProcessor.h"
#include <iostream>
#include <windows.h>

void changeColor(int desiredColor) 
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), desiredColor);
}

void BMPProcessor::OpenBMP(const std::string& fileName)
{
	if (f.is_open())
		CloseBMP();
	f.open(fileName, std::ios::in | std::ios::binary);

	if (!f.is_open())
	{
		std::cout << "File could not be opened\n";
		return;
	}


	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;

	unsigned char fileHeader[fileHeaderSize];
	f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

	if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
	{
		std::cout << "Not a .bmp file\n";
		return;
	}

	unsigned char informationHeader[informationHeaderSize];
	f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
	m_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
	m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

	m_colors.resize(m_width * m_height);

	const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			unsigned char color3[3];
			unsigned char color4[4];
			bool black;
			switch (informationHeader[14])
			{
			case 24:
				f.read(reinterpret_cast<char*>(color3), 3);
				black = !color3[1];
				m_colors[y * m_width + x].black = black;
				break;
			case 32:
				f.read(reinterpret_cast<char*>(color4), 4);
				black = !color4[1];
				m_colors[y * m_width + x].black = black;
				break;
			}
		}
		if (informationHeader[14] == 24)
			f.ignore(paddingAmount);
	}

	std::cout << "File read!\n";
}

void BMPProcessor::DisplayBMP()
{
	for (int y = m_height - 1; y >= 0; y--)
	{
		for (int x = 0; x < m_width; x++)
		{
			if (m_colors[y * m_width + x].black)
				changeColor(0);
			else
				changeColor(255);
			std::cout << "#";
		}
		std::cout << "\n";
	}
	changeColor(31);
}

void BMPProcessor::CloseBMP()
{
	f.close();
	m_colors.clear();
}
