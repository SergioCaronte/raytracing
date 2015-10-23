#ifndef OBJECT_H
#define OBJECT_H

#include "structs.h"
#include "ray.h"
#include "math/plane.h"
#include "math/matrix.h"

// Types of objects.
enum ObjectType 
{
    Sphere,
    Polyhedron,
	Torus,
	Cylinder
};

// Base Object.
class Object 
{
public:
	Object(){}

	virtual void calculate_matrices(float dt = 0) {}
	virtual float hit_test(const Ray &ray, Vector &normal, const Point *endPos = NULL, bool *inside = NULL){return -1.0;}
	Vector mul_vec( Matrix4x4 &mat, const Vector &p);
	Point mul_point( Matrix4x4 &mat, const Point &p);
    Color get_color(const Point &p);
	
	size_t id;
    ObjectType type;
	Texture texture;
	Material material;
	
	Vector acceleration;
	Point pos;
	// affines transforms
	Point original_pos;
	Point original_rot;
    Point original_scale;
	Matrix4x4 inv_trans;
};


// Sphere object.
class SphereObject : public Object
{
public:
	SphereObject();

	void calculate_matrices(float dt = 0) override;

	float hit_test(const Ray &ray, Vector &normal, const Point *max_pos = NULL, bool *inside = NULL) override;    
	// sphere radius.
    float radius;
};

// Polyhedron object.
class PolyhedronObject : public Object
{
public:
	PolyhedronObject();

	void calculate_matrices(float dt = 0) override;

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

	void calculate_matrices(float dt = 0) override;

	float hit_test(const Ray &ray, Vector &normal, const Point *max_pos = NULL, bool *inside = NULL) override;
	// torus radius
	double radius;
	// torus thickness
	double thickness;

private:
	Vector compute_normal(const Point& p);
};

class CylinderObject : public Object
{
public:
	CylinderObject();

	void calculate_matrices(float dt = 0) override;

	float hit_test(const Ray &ray, Vector &normal, const Point *max_pos = NULL, bool *inside = NULL) override;

	double bottom;
	double top;
	double radius;

private:
	Point bottom_pos;
	Point up_pos;
	double inv_radius;

	float hit_cylinder(const Ray &ray, Vector &normal, bool &inside);
	float hit_disk(const Ray &ray, Vector &normal, Vector disk_normal, Point disk_pos);

};

#endif