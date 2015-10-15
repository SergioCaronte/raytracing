#ifndef OBJECT_H
#define OBJECT_H

#include "structs.h"
#include "ray.h"
#include "math\plane.h"

// Types of objects.
enum ObjectType 
{
    SphereObjectType,
    PolyhedronObjectType
};

// Base Object.
class Object 
{
public:
	Object(){}

	virtual float hit_test(const Ray &ray, Vector &normal, const Point *endPos = NULL, bool *inside = NULL){return -1.0;}
    size_t id;
    ObjectType type;
	Texture texture;
	Material material;
    // Returns the object surface color at a given point.
    Color get_color(const Point &p);
};


// Sphere object.
class SphereObject : public Object
{
public:
	SphereObject();

	float hit_test(const Ray &ray, Vector &normal, const Point *max_pos = NULL, bool *inside = NULL) override;
    // Position of the sphere.
    Point pos;
    // Radius of the sphere.
    float radius;
};

// Polyhedron object.
class PolyhedronObject : public Object
{
public:
	PolyhedronObject();

	float hit_test(const Ray &ray, Vector &normal, const Point *max_pos = NULL, bool *inside = NULL) override;
    // Number of faces.
    size_t numFaces;
    // Faces
    std::vector<Plane> planes;
};

#endif