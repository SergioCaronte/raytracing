#include "object.h"
#include "scene.h"

//////////////////////////////////////////////////////////
/// Object class
//////////////////////////////////////////////////////////
Color Object::get_color(const Point &p)
{
	switch(texture.type) 
	{
        case SolidTexture:
            return texture.solid.color;
		break;
        case CheckerTexture:
			int pattern;
            pattern =	floor(p.x / texture.checker.scale) +
						floor(p.y / texture.checker.scale) +
						floor(p.z / texture.checker.scale);
            if(pattern % 2)
                return texture.checker.color2;
            else
                return texture.checker.color1;
		break;
        case MapTexture:
			double s, r;
			int u, v;
            //texture interpolation
			s =	texture.map.p0.x * p.x +
                texture.map.p0.y * p.y +
                texture.map.p0.z * p.z +
                texture.map.p0.w;
            r =	texture.map.p1.x * p.x +
                texture.map.p1.y * p.y +
                texture.map.p1.z * p.z +
                texture.map.p1.w;
            u = (int)(r * texture.map.image.height) % texture.map.image.height;
            v = (int)(s * texture.map.image.width) % texture.map.image.width;
            if(u < 0) u += texture.map.image.height;
            if(v < 0) v += texture.map.image.width;
            return texture.map.image.data.at(u).at(v);
		break;
    }
    throw std::runtime_error("Invalid texture type");
}

//////////////////////////////////////////////////////////
/// Sphere class
//////////////////////////////////////////////////////////

SphereObject::SphereObject()
{
	type = ObjectType::SphereObjectType;
}

float SphereObject::hit_test(const Ray &ray, Vector &normal, const Point *max_pos, bool *inside)
{
	Vector e = (ray.origin - pos);
    double a = Vector::dot(ray.direction, ray.direction);
    double b = 2.0f * Vector::dot(ray.direction, e);
    double c = Vector::dot(e, e) - std::pow(radius, 2);
    double delta = std::pow(b, 2) - 4*a*c;
    
	if(delta < FLT_EPSILON) 
        return -1.0f;
    
    // Resolving the equation.
    delta = std::sqrt(delta);

    // Test both solutions.
    double t1 = (-b - delta) / (2.0f * a);
    double t2 = (-b + delta) / (2.0f * a);

    // Calculate the maximum t.
    double max_t;
    if(max_pos)
        max_t = (max_pos->x - ray.origin.x) / ray.direction.x;
    else
        max_t = FLT_MAX;

    // t1 is always smaller than t2 because it uses -b -discriminant.
    if(t1 > FLT_EPSILON && t1 < max_t) 
	{
        if(inside)	*inside = false;
		Point intersection = ray.origin + t1 * ray.direction;
		normal = (intersection - pos).normalize();
        return t1;
    }
    else if(t2 > FLT_EPSILON && t2 < max_t) 
	{
        if(inside)	*inside = true;
		Point intersection = ray.origin + t2* ray.direction;
		normal = (intersection - pos).normalize();
        return t2;
    }
    else
        return -1.0f;
}

//////////////////////////////////////////////////////////
/// Polyhedron class
//////////////////////////////////////////////////////////

PolyhedronObject::PolyhedronObject()
{
	type = ObjectType::PolyhedronObjectType;
}

float PolyhedronObject::hit_test(const Ray &ray, Vector &normal, const Point *max_pos, bool *inside)
{
    float t0 = 0.0f, t1 = FLT_MAX;
    Vector n_t0, n_t1;

	// run for every face
    for(size_t i = 0; i < planes.size(); ++i) 
	{
        Vector p0(ray.origin.x, ray.origin.y, ray.origin.z);
        Vector n = planes[i].normal();
        float dn = Vector::dot(ray.direction, n); 
        float vd = Vector::dot(p0, n) + planes[i].d; 

        if((dn <= FLT_EPSILON && dn >= -FLT_EPSILON) && vd > FLT_EPSILON) 
		{
			t1 = -1.0f;
        }
        else if(dn > FLT_EPSILON && t1 > -vd / dn) 
		{
			 t1 = -vd / dn;
             n_t1 = n;
        }
        else if(dn < -FLT_EPSILON && t0 < -vd / dn) 
		{
            t0 = -vd / dn;
            n_t0 = n;    
        }
    }

    // Calculate the maximum t.
    double max_t;
    if(max_pos)
        max_t = (max_pos->x - ray.origin.x) / ray.direction.x;
    else
        max_t = FLT_MAX;

    if(t1 < t0)
	{
		return -1.0f;
	}
    else if(std::abs(t0) <= FLT_EPSILON && (t1 >= t0) && t1 < max_t) 
	{
        normal = (n_t1 * (-1)).normalize();
        return t1;
    }
    else if(t0 > FLT_EPSILON && t1 >= t0 && t0 < max_t) 
	{
        normal = n_t0.normalize();
        return t0;
    }
    return -1.0f;
}