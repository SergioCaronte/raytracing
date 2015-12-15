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
#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP

#include "color.h"
#include "ray.h"
#include "math/point.h"
#include "math/vector.h"
#include "structs.h"
#include "scene.h"
#include "multijittered.h"

// intersection structure
struct Intersection
{
	Point contact;	// intersection point
	Vector normal;	// normal at intersection point
    Object object;	// intersected object
};

class Raytracer
{
public:
	Raytracer(int max_ray_travel = 4){ max_depth = max_ray_travel;}
	~Raytracer(){}

	// compute raytracing. trace a ray for every pixel
	void compute(Scene &scene) ;
	// trace the ray path, raytracing core
	Color trace(Scene &scene, const Ray &ray, size_t depth, Object *excluded_obj = NULL);
	// get the ideal reflection direction
	Vector get_reflection_direction(const Vector &dir, const Vector &normal);
	// get the transmission direction
	bool get_transmission_direction(double refrRate, const Vector &dir, const Vector &normal, Vector &transDir);
	// check the ray intersection with the scene
	bool intersection(Scene &scene, const Ray &ray, Intersection &interc, 
		const Point *max_pos = NULL, const Object *excluded_obj = NULL,
        bool *inside = NULL);

private:

	MultiJittered sampler;
	// max depth a ray can go recursively
	int max_depth;

	void compute_regular(Scene &scene);
	void compute_sampled(Scene &scene);
	inline Vector get_ray_direction(Scene &scene, int w, int h, const Point &ori,const Point &lp, const Point &sp);
	inline Vector get_ray_direction(Scene &scene, int w, int h);

};
#endif // !RAYTRACER_HPP
