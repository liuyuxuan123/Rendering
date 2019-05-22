#pragma once
#include <vector>
#include "lodepng.h"
using namespace std;
class PngImage
{
public:
	PngImage();
	PngImage(char *pathname);
	~PngImage();
	void loadImage(char *pathname);
	int getWidth();
	int getHeight();
	void getRGB(int x, int y, int &r, int &g, int &b);
private:	
	vector<unsigned char> img;
	unsigned int width, height;
};

