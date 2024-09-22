#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "BMPGenerator.h"

class BMPProcessor
{
	int m_width;
	int m_height;
	std::ifstream  f;
	std::vector<Color> m_colors;
public:
	void OpenBMP(const std::string& fileName);
	void DisplayBMP();
	void CloseBMP();
};

