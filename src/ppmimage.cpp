#include "ppmimage.h"
#include "error.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

void PPMImage::create(int width, int height){}

void PPMImage::set_color(int w, int h, Color c){}

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
        throw std::runtime_error("Ppm sizes are invalid");
    if(maxColor > 255)
        throw std::runtime_error("Invalid maxColor (max is 255)");

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
                fatalError("Bad ppm file");

            c.r = (float) r / maxColor;
            c.g = (float) g / maxColor;
            c.b = (float) b / maxColor;
            data.at(i).push_back(c);
        }
    }
    in.close();
}
