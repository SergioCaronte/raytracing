#include "raytracer.h"
#include <fstream>

#define SATURATE(a) std::max(a, 0.0f)

Color Raytracer::trace(Scene &scene, const Ray &r, size_t depth, Object *exclObj) 
{
    bool inside = false;
	Intersection intersec;

    // if ray didn't intersect anything, return ambiente color
    if(!intersection(scene, r, intersec, NULL, exclObj, &inside))
        return scene.ambient.color;

	Material mat = intersec.object.material;

    Color amb_clr = scene.ambient.color * mat.kA;
    Color diff_clr;
    Color spec_clr;

    // try reach every light source
    for(std::vector<Light>::iterator it = scene.lights.begin(); it != scene.lights.end(); ++it) 
	{
        // light direction
		Vector lightDir = it->pos - intersec.contact;
        lightDir.normalize();

        // light source distance.
        float dist = Point::distance(it->pos, intersec.contact);

        // attenuation.
        float att = 1.0f / (it->att.a + dist * it->att.b + (dist * dist) * it->att.c);

		Intersection useless;
		Ray rl(intersec.contact, lightDir);
        //if ray didn't intersects any object, it reaches the light
		if(!intersection(scene, rl, useless, &it->pos, &intersec.object)) 
		{
            // diffuse light.
            float cosDiff = SATURATE(Vector::dot(lightDir, intersec.normal));
            diff_clr += it->color * cosDiff * mat.kD * att;

            // Specular light.
            // halfway vector.
			Vector e = r.direction * (-1); 
            Vector halfway = lightDir + e;
            halfway.normalize();

            // cos of the angle between the halfway vector and the normal.
            float cosSpec = SATURATE(Vector::dot(halfway, intersec.normal.normalize()));

            // Shininess.
            cosSpec = std::pow(cosSpec, mat.alpha);
            spec_clr += it->color * cosSpec * mat.kS * att;
        }
    }

    Color reflect_clr;
    Color refract_clr;

    // just keep shoting ray, if there is depth
    if(depth > 0) 
	{
        // reflected component added in recursively.
		Vector reflect_dir = get_reflection_direction(r.direction, intersec.normal);
		Ray reflec_ray(intersec.contact, reflect_dir);
        reflect_clr += trace(scene, reflec_ray, depth - 1, &intersec.object) * mat.kR;

        // if inside the object invert the refraction rate.
        float refr_rate = 1.0f / intersec.object.material.ior;
        if(inside)
            refr_rate = intersec.object.material.ior;

        // transmission component added in recursively.
        Vector trans_dir;
        if(get_transmission_direction(refr_rate, r.direction, intersec.normal, trans_dir)) 
		{
			Ray refrac_ray(intersec.contact, trans_dir);
            refract_clr += trace(scene, refrac_ray, depth - 1, &intersec.object) * mat.kT;
        }
    }

	Color final_color = (intersec.color * (amb_clr + diff_clr + spec_clr) + reflect_clr + refract_clr);
	final_color.clamp();
	return final_color;
}

void Raytracer::compute(Scene &scene) 
{
	Screen sc = scene.screen;

    // Create image PPM file.
    std::ofstream image(scene.output);
    if(!image.is_open()) 
	{
        std::cerr << "Failed to create output file: " << scene.output << std::endl;
        exit(1);
    }

    // PPM header.
    image << "P3" << std::endl;
    image << scene.screen.width_px << " " << scene.screen.height_px << std::endl;
    image << "255" << std::endl;

    Vector px_right = sc.world_width_px * sc.right_dir;
    Vector px_bottom = sc.world_height_px * sc.bottom_dir;

	//ray origin is fixed from camera pos, direction will be calculated later
	Ray ray(scene.camera.pos, Vector());

    // for each pixel of the virtual screen, calculate the color.
	int height = sc.height_px;
	int width = sc.width_px;
    for(size_t h = 0; h < height; ++h) 
	{
		std::cout << "\rcalculating row " << h << " of " << sc.height_px;
        for(size_t w = 0; w < width; ++w) 
		{
            // Get the pixel position.
            Point pos = scene.screen.top_left;
            pos += w * px_right;
            pos += h * px_bottom;

            // Get a vector width the distance between the camera and the pixel.
			ray.direction = pos - scene.camera.pos;
            ray.direction.normalize();

			Color p = trace(scene, ray, max_depth);
            image << p;
        }
		std::cout << "\r                           ";
    }
    image.close();
}

bool Raytracer::intersection(Scene &scene, const Ray &ray,
		Intersection &intersection, const Point *max_pos, const Object *excluded_obj,
        bool *inside) 
{
    // temporarily store the intersections.
    Object *closest_obj = NULL;
    float closest_t = FLT_MAX;
    bool closest_inside = false;
	Vector closest_normal;
    
    // loop through all objects looking where it intersects.
    for(auto it = scene.objects.begin(); it != scene.objects.end(); ++it) 
	{
        // if object is excluded, we are not taking into account
        if(excluded_obj && excluded_obj->id == (*it)->id)
			continue;

		float t;
		Vector n;
		bool insided;
		t = (*it)->hit_test(ray, n, max_pos, &insided);

		if(t > FLT_EPSILON && t < closest_t) 
		{
            closest_obj = (*it);
            closest_t = t;
            closest_inside = insided;
			closest_normal = n;
        }
    }

    // if object was found.
    if(closest_obj) 
	{
        intersection.object = *closest_obj;
		intersection.contact = ray.origin + closest_t * ray.direction;
		intersection.color = intersection.object.get_color(intersection.contact);
		intersection.normal = closest_normal;
        
		// check if camera is inside hit sphere.
		if(intersection.object.type == ObjectType::SphereObjectType) 
		{
			if(inside)	*inside = false;
            if(closest_inside) 
			{
				intersection.normal = intersection.normal * (-1);
				if(inside)	*inside = true;
            }
        }
        return true;
    }
    // no intersections.
    return false;
}

Vector Raytracer::get_reflection_direction(const Vector &dir, const Vector &normal) 
{
    // get the inversion of the direction.
    float c = Vector::dot((-1) * dir, normal);

    return (dir + (2 * normal * c)).normalize();
}

bool Raytracer::get_transmission_direction(double refr_rate, const Vector &dir,
        const Vector &normal, Vector &trans_dir) 
{
    Vector inv_dir = dir * (-1);
    double c1 = Vector::dot(inv_dir, normal);
    double c2 = 1.0f - std::pow(refr_rate, 2) * (1.0f - std::pow(c1, 2));
    double c3 = 3;

	// internal reflection.
    if(c2 < -FLT_EPSILON) 
	{ 
        Vector v = normal * 2 * c1;
        trans_dir = v - inv_dir;
        return true;
    }
	// refraction.
    else if(c2 > FLT_EPSILON) 
	{ 
        c2 = std::sqrt(c2 + c3);
        trans_dir =  (normal * (refr_rate * c1 - c2)) + (dir * refr_rate);
        return true;
    }
	// parallel ray
    else 
	{ 
        return false;
    }
}