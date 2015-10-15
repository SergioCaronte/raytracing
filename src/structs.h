#ifndef STRUCTS_H
#define STRUCTS_H

#include "math/vector.h"
#include "math/point.h"
#include "color.h"
#include "ppmimage.h"
// Structs for Raytracing task

// Camera.
struct Camera 
{
    Point pos;	// Position point.
    Vector dir;	// Direction vector.
    Vector up;	// Up vector.
    float fovy; // Field of view, in degrees.
};

// Screen display.
struct Screen 
{  
    Point center;		// Coordinate of the center of the display screen.
	Vector right_dir;	// Direction to the right of the screen.
	Vector bottom_dir;	// Direction to the bottom of the screen.
	Point top_left;		// Position of the top left pixel.
    size_t width_px;	// Size in pixels of the width of the screen.
    size_t height_px;	// Size in pixels of the height of the screen.
	float width;		// Size in world coordinates of the width of the screen.
	float height;		// Size in world coordinates of the height of the screen.
	float world_width_px;	// Size in world coordinates of the width of a pixel.
	float world_height_px;	// Size in world coordinates of the height of a pixel.
};

// Attenuation factor of the light.
struct Attenuation 
{
    float a;	// Constant attenuation.  
    float b;	// Linear attenuation.
    float c;	// Quadratic attenuation.
};

// Light.
struct Light
{
    Point pos;			//Light position
    Color color;		//Light color
    Attenuation att;	//Attenuation factor
};

// Types of textures.
enum TextureType 
{
    SolidTexture,
    CheckerTexture,
    MapTexture
};

// solid texture data.
struct SolidTextureData 
{
    Color color;	// Color of the entire texture.
};

// checker texture data.
struct CheckerTextureData 
{
    Color color1;
    Color color2;
    Color map[2][2];
    float scale;
};

// map texture data.
struct MapTextureData 
{
    std::string filename;
    Point p0;
    Point p1;
    PPMImage image;
};

// texture.
struct Texture 
{
    int id;						// ID of the texture.
    TextureType type;			// Type of the texture.
    SolidTextureData solid;		// Data related to a solid texture.
    CheckerTextureData checker;	// Data related to a checker texture.
    MapTextureData map;			// Data related to a map texture.
};

// material.
struct Material 
{
    int id;
    // Material coefficients
    float kA;		// ambient
    float kD;		// diffuse
    float kS;		// specular
    float alpha;	// alpha
    float kR;		// reflection
    float kT;		// transmission
    float ior;		// transmission factor
};

#endif
