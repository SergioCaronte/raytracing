#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP

#include "color.h"
#include "ray.h"
#include "math\point.h"
#include "math\vector.h"
#include "structs.h"
#include "scene.h"

// intersection structure
struct Intersection
{
	Point contact;	// intersection point
	Color color;	// color at intersection point
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
	Color trace(Scene &scene, const Ray &ray, size_t depth, Object *exclObj = NULL);
	// get the ideal reflection direction
	Vector get_reflection_direction(const Vector &dir, const Vector &normal);
	// get the transmission direction
	bool get_transmission_direction(double refrRate, const Vector &dir, const Vector &normal, Vector &transDir);
	// check the ray intersection with the scene
	bool intersection(Scene &scene, const Ray &ray, Intersection &interc, 
		const Point *max_pos = NULL, const Object *excluded_obj = NULL,
        bool *inside = NULL);
	// max depth a ray can go recursively
	int max_depth;

};
#endif // !RAYTRACER_HPP
