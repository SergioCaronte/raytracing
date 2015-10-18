#ifndef OBJECT_H
#define OBJECT_H

#include "structs.h"
#include "ray.h"
#include "math\plane.h"
#include "math\matrix.h"

// Types of objects.
enum ObjectType 
{
    Sphere,
    Polyhedron,
	Torus
};

// Base Object.
class Object 
{
public:
	Object(){}

	virtual float hit_test(const Ray &ray, Vector &normal, const Point *endPos = NULL, bool *inside = NULL){return -1.0;}
	Vector mul_vec( Matrix4x4 &mat, const Vector &p);
	Point mul_point( Matrix4x4 &mat, const Point &p);
    
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
    // sphere center.
    Point pos;
    // sphere radius.
    float radius;
};

// Polyhedron object.
class PolyhedronObject : public Object
{
public:
	PolyhedronObject();

	float hit_test(const Ray &ray, Vector &normal, const Point *max_pos = NULL, bool *inside = NULL) override;
    // number of faces.
    size_t numFaces;
    // faces
    std::vector<Plane> planes;
};

class TorusObject : public Object
{
public:
	TorusObject();

	void calculate_matrices();

	float hit_test(const Ray &ray, Vector &normal, const Point *max_pos = NULL, bool *inside = NULL) override;
	// torus radius
	double radius;
	// torus thickness
	double thickness;
	//
	Point pos;
	Point rot;

	Matrix4x4 transform;
	Matrix4x4 inv_trans;

private:
	Vector compute_normal(const Point& p);
};

#endif