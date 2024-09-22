#include "BMPGenerator.h"
#include <iostream>
#include <fstream>

Color::Color()
	: black(false)
	// : red(0), green(0), blue(0)
{
}

/*
Color::Color(float red, float green, float blue)
	: red(red), green(green), blue(blue)
{
}
*/

Color::Color(bool black)
	: black(black)
{
}

Color::~Color()
{
}

BMPGenerator::BMPGenerator(int width, int height)
	: m_width(width), m_height(height), m_colors(std::vector<Color>(width * height))
{
}

BMPGenerator::~BMPGenerator()
{
}

Color BMPGenerator::GetColor(int x, int y) const
{
	return m_colors[y * m_width + x];
}

void BMPGenerator::SetColor(const Color& color, int x, int y)
{
	/*
	m_colors[y * m_width + x].red = color.red;
	m_colors[y * m_width + x].green = color.green;
	m_colors[y * m_width + x].blue = color.blue;
	*/

	m_colors[y * m_width + x].black = color.black;
}

void BMPGenerator::Generate(int blackPercentage)
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int temp = rand() % 101;
			if (temp < blackPercentage)
				SetColor(Color(true), x, y);
			else
				SetColor(Color(false), x, y);
		}
	}
}

void BMPGenerator::Export(const char* path, int bits) const
{
	std::ofstream  f;
	f.open(path, std::ios::out | std::ios::binary);

	if (!f.is_open())
	{
		std::cout << "File could not be opened\n";
		return;
	}

	unsigned char bmppad[3] = { 0, 0, 0 };

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int colorHeader = 84; // for 32 bit only
	int paddingAmount;
	int fileSize;

	switch (bits)
	{
	case 24:
		paddingAmount = ((4 - (m_width * 3) % 4) % 4);
		fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmount * m_height;
		break;
	case 32:
		paddingAmount = 0;
		fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 4;
		break;
	default:
		std::cout << "Incorrect bits amount (" << bits << "), only 24 or 32 are possible\n";
		return;
	}

	unsigned char fileHeader[fileHeaderSize];

	//File type
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	switch (bits)
	{
	case 24:
		fileHeader[2] = fileSize; //File Size
		fileHeader[3] = fileSize >> 8;
		fileHeader[4] = fileSize >> 16;
		fileHeader[5] = fileSize >> 24;
		fileHeader[10] = fileHeaderSize + informationHeaderSize; // Pixel data offset (1st part)
		break;
	case 32:
		fileHeader[2] = 186; //File Size
		fileHeader[3] = 6;
		fileHeader[4] = 0;
		fileHeader[5] = 0;
		fileHeader[10] = 122; // Pixel data offset (1st part)
		break;
	}
	// Reserved 1 (Not used)
	fileHeader[6] = 0;
	fileHeader[7] = 0;
	// Reserved 2 (Not used)
	fileHeader[8] = 0;
	fileHeader[9] = 0;
	// Pixel data offset (2nd part)
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];

	switch (bits)
	{
	case 24:
		informationHeader[0] = informationHeaderSize; // Header size
		informationHeader[14] = 24; // Bits per pixel (RGB)
		informationHeader[16] = 0; // Compression (No compression)		
		informationHeader[20] = 0; // Image size (No compression)
		informationHeader[21] = 0;
		informationHeader[24] = 0; // X pixels per meter (Not specified)
		informationHeader[25] = 0;
		informationHeader[28] = 0; // Y pixels per meter (Not specified)
		informationHeader[29] = 0;
		break;
	case 32:
		informationHeader[0] = 108; // Header size
		informationHeader[14] = 32; // Bits per pixel (RGBA)
		informationHeader[16] = 3; // Compression (For RGBA)
		informationHeader[20] = 186; // Image size (W/ compression)
		informationHeader[21] = 6;
		informationHeader[24] = 196; // X pixels per meter
		informationHeader[25] = 14;
		informationHeader[28] = 196; // Y pixels per meter
		informationHeader[29] = 14;
		break;
	}
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;

	// Image width
	informationHeader[4] = m_width;
	informationHeader[5] = m_width >> 8;
	informationHeader[6] = m_width >> 16;
	informationHeader[7] = m_width >> 24;

	// Image height
	informationHeader[8] = m_height;
	informationHeader[9] = m_height >> 8;
	informationHeader[10] = m_height >> 16;
	informationHeader[11] = m_height >> 24;

	// Planes
	informationHeader[12] = 1;
	informationHeader[13] = 0;

	informationHeader[15] = 0;

	informationHeader[17] = 0;
	informationHeader[18] = 0;
	informationHeader[19] = 0;

	informationHeader[22] = 0;
	informationHeader[23] = 0;

	informationHeader[26] = 0;
	informationHeader[27] = 0;

	informationHeader[30] = 0;
	informationHeader[31] = 0;

	// Total colors (Color pallete not used)
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;

	// Important colors (Generally ignored)
	informationHeader[36] = 0;
	informationHeader[37] = 0;
	informationHeader[38] = 0;
	informationHeader[39] = 0;

	f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			/*
			unsigned char r = static_cast<unsigned char>(GetColor(x, y).red * 255.0f);
			unsigned char g = static_cast<unsigned char>(GetColor(x, y).green * 255.0f);
			unsigned char b = static_cast<unsigned char>(GetColor(x, y).blue * 255.0f);
			*/

			unsigned char rgb; // Suitable for black, white and other (x, x, x) colors

			if (GetColor(x, y).black)
				rgb = static_cast<unsigned char>(0);
			else
				rgb = static_cast<unsigned char>(255);

			int alpha = rand() % 256;

			// unsigned char color3[] = { b, g, r };
			// unsigned char color4[] = { b, g, r, alpha};
			unsigned char color3[] = { rgb, rgb, rgb };
			unsigned char color4[] = { rgb, rgb, rgb, alpha };
			switch (bits)
			{
			case 24:
				f.write(reinterpret_cast<char*>(color3), 3);
				break;
			case 32:
				f.write(reinterpret_cast<char*>(color4), 4);
				break;
			}
		}
		if (bits == 24)
			f.write(reinterpret_cast<char*>(bmppad), paddingAmount);
	}

	f.close();

	std::cout << "File created!\n";
}
