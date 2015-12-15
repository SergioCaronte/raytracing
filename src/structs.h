/*
* Copyright (C) 2015 Sergio Nunes da Silva Junior 
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 of the License.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* author: Sergio Nunes da Silva Junior
* contact: sergio.nunes@dcc.ufmg.com.br
* Universidade Federal de Minas Gerais (UFMG) 
*/
#ifndef STRUCTS_H
#define STRUCTS_H

#include "math/vector.h"
#include "math/point.h"
#include "color.h"
#include "ppmimage.h"

// Structs for Raytracing task

struct Sampler;

// Camera.
struct Camera 
{
    Point pos;	// Position point.
	Point lookat; // Look at position
    Vector dir;	// Direction vector.
    Vector up;	// Up vector.
	Vector x, y, z;
    float fovy; // Field of view, in degrees.
	float shutter_time; //  Time that lens is opened to capture light
	float exposure;		//exposure value
	
	Sampler* sampler;
	float focal_dist;
	float lens_radius;
};

// Screen display.
struct Screen 
{  
    size_t width_px;	// Size in pixels of the width of the screen.
    size_t height_px;	// Size in pixels of the height of the screen.
	float d;
	int samples;
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
