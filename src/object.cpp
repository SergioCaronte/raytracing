#include "object.h"
#include "scene.h"
#include "math\math.h"


#define	IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)

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
}

Vector Object::mul_vec( Matrix4x4 &mat, const Vector &p)
{
	return Vector(mat[0][0] * p.x + mat[0][1] * p.y + mat[0][2] * p.z,
					mat[1][0] * p.x + mat[1][1] * p.y + mat[1][2] * p.z,
					mat[2][0] * p.x + mat[2][1] * p.y + mat[2][2] * p.z);
}

Point Object::mul_point( Matrix4x4 &mat, const Point &p)
{
	return Point(mat[0][0] * p.x + mat[0][1] * p.y + mat[0][2] * p.z + mat[0][3],
					mat[1][0] * p.x + mat[1][1] * p.y + mat[1][2] * p.z + mat[1][3],
					mat[2][0] * p.x + mat[2][1] * p.y + mat[2][2] * p.z + mat[2][3]);
}

//////////////////////////////////////////////////////////
/// Sphere class
//////////////////////////////////////////////////////////

SphereObject::SphereObject()
{
	type = ObjectType::Sphere;
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
	type = ObjectType::Polyhedron;
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

/////////////////////////////////////////
/// Cylinder
/////////////////////////////////////////
TorusObject::TorusObject()
{
	type = ObjectType::Torus;
}

void TorusObject::calculate_matrices()
{
	rot.x = toRads(rot.x);
	rot.y = toRads(rot.y);
	rot.z = toRads(rot.z);

	//translate and inverse translate matrices
	Matrix4x4 t, i_t;
	t.identity();
	t[0][3] = pos.x;	t[1][3] = pos.y;	t[2][3] = pos.z;

	i_t.identity();
	i_t[0][3] = -pos.x;	i_t[1][3] = -pos.y;	i_t[2][3] = -pos.z;

	//rotation and inverse rotation matrices
	Matrix4x4 r, i_r;
	i_r.identity();
	i_r[0][0] = cos(rot.y)*cos(rot.z);	i_r[0][1] = sin(rot.z);					i_r[0][2] = -sin(rot.y);
	i_r[1][0] = -sin(rot.z);			i_r[1][1] = cos(rot.x)*cos(rot.z);		i_r[1][2] = sin(rot.x);
	i_r[2][0] = sin(rot.y);				i_r[2][1] = -sin(rot.x);				i_r[2][2] = cos(rot.x)*cos(rot.y);

	r.identity();
	r[0][0] = cos(rot.y)*cos(rot.z);	r[0][1] = -sin(rot.z);				r[0][2] = sin(rot.y);
	r[1][0] = sin(rot.z);				r[1][1] = cos(rot.x)*cos(rot.z);	r[1][2] = -sin(rot.x);
	r[2][0] = -sin(rot.y);				r[2][1] = sin(rot.x);				r[2][2] = cos(rot.x)*cos(rot.y);

	Matrix4x4 i_rx, i_ry, i_rz;
	i_rx.identity();
	i_rx[1][1] = cos(rot.x);		i_rx[1][2] = sin(rot.x);
	i_rx[2][1] = -sin(rot.x);		i_rx[2][2] = cos(rot.x);

	i_ry.identity();	
	i_ry[0][0] = cos(rot.y);		i_ry[0][2] = -sin(rot.y);
	i_ry[2][0] = sin(rot.y);		i_ry[2][2] = cos(rot.y);

	i_rz.identity();	
	i_rz[0][0] = cos(rot.z);		i_rz[0][1] = sin(rot.z);
	i_rz[1][0] = -sin(rot.z);		i_rz[1][1] = cos(rot.z);

	//object transform matrix  t rz rx ry
	transform = t * r;

	//object inverse transform matrix
	inv_trans = i_t * i_r;
	//inv_trans = i_ry * i_rx * i_rz * i_t;
	//inv_trans = i_t * i_rz * i_rx * i_ry;
}

float TorusObject::hit_test(const Ray &ray, Vector &normal, const Point *max_pos, bool *inside)
{
	Ray inv_ray;
	inv_ray.origin = mul_point(inv_trans, ray.origin);
	inv_ray.direction = mul_vec(inv_trans, ray.direction);

	double x1 = inv_ray.origin.x; double y1 = inv_ray.origin.y; double z1 = inv_ray.origin.z;
	double d1 = inv_ray.direction.x; double d2 = inv_ray.direction.y; double d3 = inv_ray.direction.z;

	double coeffs[5];	// coefficient array
	double roots[4];	// solution array

	//define the coefficients
	double sum_d_sqrd = d1*d1 + d2*d2 + d3*d3;
	double e = x1*x1 + y1*y1 + z1*z1 - radius*radius - thickness*thickness;
	double f = x1*d1 + y1*d2 + z1*d3;
	double four_a_sqrd = 4.0 * radius*radius;

	coeffs[0] = e*e - four_a_sqrd * (thickness*thickness-y1*y1);	// constante term
	coeffs[1] = 4.0 * f * e + 2.0 * four_a_sqrd * y1 *d2;
	coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
	coeffs[3] = 4.0 * sum_d_sqrd * f;
	coeffs[4] = sum_d_sqrd * sum_d_sqrd;	// coefficient of t^4

	//fin the roots
	int num_real_roots = solveQuartic(coeffs, roots);

	bool intersected = false;
	double t = FLT_MAX;

	if ( num_real_roots == 0)	// ray misses the torus
		return -1.0;

	//find the smallest root greater than FLT_EPSILON, if any
	for( int j = 0; j < num_real_roots; j++)
	{
		if(roots[j] > FLT_EPSILON)
		{
			intersected = true;
			if(roots[j] < t)
				t = roots[j];
		}
	}

	double max_t;
    if(max_pos)
        max_t = (max_pos->x - ray.origin.x) / ray.direction.x;
    else
        max_t = FLT_MAX;

	if( t > max_t || !intersected)
		return -1.0;


	Point hit = inv_ray.origin + t*inv_ray.direction; 
	normal = compute_normal(hit);
	normal = mul_vec(inv_trans, normal);
	normal.normalize();
	return t;
}

Vector TorusObject::compute_normal(const Point& p) 
{
	Vector normal;
	double param_squared = radius * radius + thickness * thickness;

	double x = p.x;
	double y = p.y;
	double z = p.z;
	double sum_squared = x * x + y * y + z * z;
	
	normal.x = 4.0 * x * (sum_squared - param_squared);
	normal.y = 4.0 * y * (sum_squared - param_squared + 2.0 * radius * radius);
	normal.z = 4.0 * z * (sum_squared - param_squared);
	normal.normalize();
	
	return (normal);	
}



