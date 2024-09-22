#pragma once

#include <string>
#include <vector>

struct Color
{
	//float red, green, blue;
	//Color(float red, float green, float blue);

	bool black;

	Color();
	Color(bool black);
	~Color();
};

class BMPGenerator
{
	int m_width;
	int m_height;
	std::vector<Color> m_colors;
public:
	BMPGenerator(int width, int height);
	~BMPGenerator();

	Color GetColor(int x, int y) const;
	void SetColor(const Color& color, int x, int y);

	void Generate(int blackPercentage = 50);
	void Export(const char* path, int bits) const;
};

