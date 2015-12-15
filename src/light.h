#ifndef LIGHT_H
#define LIGHT_H

#include "math/point.h"
#include "color.h"

class Sampler;

enum LightAreaType
{
	LAT_NoArea,
	LAT_Square,
	LAT_Disk,
	LAT_Sphere,
	LAT_Hemisphere
};

// Attenuation factor of the light.
struct Attenuation 
{
    float a;	// Constant attenuation.  
    float b;	// Linear attenuation.
    float c;	// Quadratic attenuation.
};

// Light.
class Light
{
public:
	Light();
	~Light();
	void build_area(std::string type);
	Point get_point();

    Point pos;			//Light position
    Color color;		//Light color
    Attenuation att;	//Attenuation factor
	Sampler* sampler;	//Sampler for light area
	int num_samples;	//Number of samples
	LightAreaType type;	//Type os areas
	float area_size;	//Light area size
	float half_area_size;
};


#endif