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
#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include "structs.h"
#include "light.h"

class Scene 
{
public:
	Scene(){}

	void load_file(int argc, char **argv);
	void compute();
    
	char *input;
    char *output;

	Screen screen;
    Camera camera;

    size_t numLights;
    Light ambient;
    std::vector<Light> lights;

    size_t numTextures;
    std::vector<Texture> textures;

	size_t numMaterials;
	std::vector<Material> materials;

    size_t numObjects;
	std::vector<Object*> objects;

protected:
	void calculate_cam_base();
	void parse_camera(std::ifstream &in);
	void parse_light(std::ifstream &in);
	void parse_texture(std::ifstream &in);
	void parse_material(std::istream &in);
	void parse_object(std::istream &in);
};
#endif 
