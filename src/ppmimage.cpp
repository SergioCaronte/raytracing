#include "ppmimage.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

void PPMImage::create(int w, int h)
{
	width = w;
	height = h;
	data.resize(height);
	for(size_t i = 0; i < data.size(); ++i)
		data[i].resize(width);
}

void PPMImage::set_color(const int &w, const int &h, Color c)
{
	data[h][w] = c;
}

bool PPMImage::save(const std::string filename)
{
	// Create image PPM file.
    std::ofstream file(filename);
    if(!file.is_open()) 
	{
        std::cerr << "Failed to create output file: " << filename << std::endl;
        return false;
    }

    // PPM header.
    file << "P3" << std::endl;
    file << width << " " << height << std::endl;
    file << "255" << std::endl;

	for(size_t h = 0; h < height; ++h) 
        for(size_t w = 0; w < width; ++w) 
			 file << data[h][w];
	file.close();
	return true;
}

void PPMImage::load(const std::string &file) 
{
    std::ifstream in(file.c_str());
    if(!in)
        throw std::runtime_error("Failed to open ppm file");

    std::string line;
    std::stringstream ss;
    int maxColor;

    getline(in, line);
    if(line != "P6")
        throw std::runtime_error("Invalid ppm file");

    do{
        getline(in, line);
    } while(line[0] == '#');

    ss << line;
    getline(in, line);
    ss << " " << line;
    ss >> width >> height >> maxColor;

    if(width <= 0 || height <= 0 || maxColor <= 0)
        throw std::runtime_error("Ppm sizes are invalid.");
    if(maxColor > 255)
        throw std::runtime_error("Invalid maxColor (max is 255).");

    data.resize(height);

    // Store all the bytes.
    unsigned char r, g, b;
    Color c;
    for(size_t i = 0; i < height; ++i) 
	{
        for(size_t j = 0; j < width; ++j) 
		{
            in >> r >> g >> b;
            if(!in.good())
                printf("Unformatted ppm.");

            c.r = (float) r / maxColor;
            c.g = (float) g / maxColor;
            c.b = (float) b / maxColor;
            data.at(i).push_back(c);
        }
    }
    in.close();
}
