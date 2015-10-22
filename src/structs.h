#ifndef STRUCTS_H
#define STRUCTS_H

#include "math/vector.h"
#include "math/point.h"
#include "color.h"
#include "ppmimage.h"

// Structs for Raytracing task

class Sampler;

// Camera.
struct Camera 
{
    Point pos;	// Position point.
	Point lookat; // Look at position
    Vector dir;	// Direction vector.
    Vector up;	// Up vector.
	Vector x, y, z;
    float fovy; // Field of view, in degrees.
	float exposure_time; // Time of exposion
	
	Sampler* sampler;
	float focal_dist;
	float lens_radius;
};

// Screen display.
struct Screen 
{  
    Point center;		// Coordinate of the center of the display screen.
	//Vector right_dir;	// Direction to the right of the screen.
	//Vector bottom_dir;	// Direction to the bottom of the screen.
	//Point top_left;		// Position of the top left pixel.
    size_t width_px;	// Size in pixels of the width of the screen.
    size_t height_px;	// Size in pixels of the height of the screen.
	//float width;		// Size in world coordinates of the width of the screen.
	//float height;		// Size in world coordinates of the height of the screen.
	float px_size_w;	// Size in world coordinates of the width of a pixel.
	float px_size_h;	// Size in world coordinates of the height of a pixel.
	float aspect;
	float d;
	int samples;
	float theta;
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
