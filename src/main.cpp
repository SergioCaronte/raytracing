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
#include "scene.h"
#include "raytracer.h"

int main(int argc, char **argv) {
    Scene scene;
	scene.load_file(argc, argv);
    
	Raytracer rt(4);
	rt.compute(scene);

	//system("pause");

    return 0;
}
