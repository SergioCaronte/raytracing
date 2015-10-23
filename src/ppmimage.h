#ifndef PPM_H
#define PPM_H
#include "color.h"
#include <string>
#include <vector>

class PPMImage 
{
public:
    PPMImage() : height(0), width(0) {}

	void create(int width, int height);
	void set_color(const int &w, const int &h, Color c);
	bool save(const std::string filename);

    inline bool loaded() {  return height == 0; }
    void load(const std::string &file);

	std::vector< std::vector<Color> > data;
    size_t height;
    size_t width;
};

#endif // !PPM_HPP
