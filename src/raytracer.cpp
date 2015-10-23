#include "raytracer.h"
#include "ppmimage.h"
#include <fstream>

#define SATURATE(a) std::max(a, 0.0f)

void Raytracer::compute(Scene &scene) 
{
	if(scene.screen.samples > 1)
		compute_sampled(scene);
	else
		compute_regular(scene);
}

Vector Raytracer::get_ray_direction(Scene &scene, int w, int h, const Point &ori, const Point &lp, const Point &sp)
{
	//float px_h = h + sp.y - lp.y;
	//float px_w = w + sp.x - lp.x;
	//
	////pixel position onto view plane
	//float ay = -scene.screen.px_size_h * ((px_h /(float)scene.screen.height_px) - 0.5f) ;
	//float ax = scene.screen.px_size_w * ((px_w /(float)scene.screen.width_px) - 0.5f) ;  
	//float az = 1;

	////az = scene.camera.focal_dist - scene.screen.d;
	////ay = (ay * scene.camera.focal_dist / scene.screen.d);
	////ax = (ax * scene.camera.focal_dist / scene.screen.d);

	//Point pt = ori + (ax * scene.camera.x) + (ay * scene.camera.y) - (az * scene.camera.z);
	//Vector dir = pt - ori;

	float px_h = h;
	float px_w = w;
	
	//pixel position onto view plane
	float ay = -1 * (px_h -(float)scene.screen.height_px/2 + sp.y);
	float ax = 1 * (px_w - (float)scene.screen.width_px/2 + sp.x);  
	float az = 1;

	az = scene.camera.focal_dist;
	ay = (ay * scene.camera.focal_dist / scene.screen.d);
	ax = (ax * scene.camera.focal_dist / scene.screen.d);

	//Point pt = ori + (ax * scene.camera.x) + (ay * scene.camera.y) - (az * scene.camera.z);
	//Vector dir = pt - ori;
	Vector dir = ((ax - lp.x) * scene.camera.x) + ((ay - lp.y) * scene.camera.y) - (az * scene.camera.z);
	dir.normalize();
	return dir;
}

Vector Raytracer::get_ray_direction(Scene &scene, int w, int h)
{
	//pixel position onto view plane
	//float ay = -scene.screen.px_size_h * (((float)h / (float)scene.screen.height_px) - 0.5f) ;
	//float ax = scene.screen.px_size_w  * (((float)w / (float)scene.screen.width_px) - 0.5f) ;  

	//Point pt = scene.camera.pos + (ax * scene.camera.x) + (ay * scene.camera.y) - scene.camera.z;
	//Vector dir = pt - scene.camera.pos;
	//return dir.normalize();

	float ay = -1 * ((float)h - (float)scene.screen.height_px/2.0 - 0.5f) ;
	float ax = 1  * ((float)w - (float)scene.screen.width_px/2.0 - 0.5f) ;  

	//Point pt = (ax * scene.camera.x) + (ay * scene.camera.y) - (scene.screen.d * scene.camera.z);
	Vector dir = (ax * scene.camera.x) + (ay * scene.camera.y) - (scene.screen.d * scene.camera.z);
	return dir.normalize();
}

void Raytracer::compute_regular(Scene &scene)
{
	Screen sc = scene.screen;
	Camera cam = scene.camera;

    //Vector px_right = sc.px_size_w * sc.right_dir;
    //Vector px_bottom = sc.px_size_h * sc.bottom_dir;

	//ray origin is fixed from camera pos, direction will be calculated later
	Ray ray(cam.pos, Vector());

	//output file
	PPMImage output;
	output.create(sc.width_px, sc.height_px);

	// for each pixel of the virtual screen, calculate the color.
    for(size_t h = 0; h < sc.height_px; ++h) 
	{
		std::cout << "calculating row " << h << " of " << sc.height_px << "\r";
		std::cout.flush();
        for(size_t w = 0; w < sc.width_px; ++w) 
		{
            // Get the pixel position.
            //Point pos = sc.top_left;
            //pos += w * px_right;
            //pos += h * px_bottom;
			

			//float ay = -nh * ((h/(float)sc.height_px) - 0.5f);
			//float ax = nw * ((w/(float)sc.width_px) - 0.5f);  
			
			//Point pt = cam.pos + (ax * cam.x) + (ay * cam.y) - cam.z;
			//Vector dir = pt - cam.pos;
			//dir.normalize();
            // Get a vector width the distance between the camera and the pixel.
			//ray.direction = (pos - scene.camera.pos).normalize();
			ray.direction = get_ray_direction(scene, w, h);

			Color p = trace(scene, ray, max_depth);
			output.set_color(w, h, p);
        }
    }
	if(!output.save(scene.output))
		std::cerr << "Failed to save output file: " << scene.output << std::endl;
}

void  Raytracer::compute_sampled(Scene &scene)
{
	int num_samples = scene.screen.samples;
	float inv_samples = 1.0/num_samples;
	sampler = MultiJittered(num_samples);
	
	Screen sc = scene.screen;

    //Vector px_right = sc.px_size_w;// * sc.right_dir;
    //Vector px_bottom = sc.px_size_h;// * sc.bottom_dir;

	//output file
	PPMImage output;
	output.create(sc.width_px, sc.height_px);
	// for each pixel of the virtual screen, calculate the color.

    for(size_t h = 0; h < sc.height_px; ++h) 
	{
		std::cout << "calculating row " << h << " of " << sc.height_px << "\r";
		std::cout.flush();
        for(size_t w = 0; w < sc.width_px; ++w) 
		{
			Color p;
			for(int j = 0; j < num_samples; j++)
			{
				Point sp = sampler.sample_unit_square();
				
				Point dp = scene.camera.sampler->sample_unit_disk();
				Point lp = dp * scene.camera.lens_radius;

				Ray ray;
				ray.origin = scene.camera.pos + lp.x * scene.camera.x + lp.y * scene.camera.y;
				// Get a vector width the distance between the camera and the pixel.
				ray.direction = get_ray_direction(scene, w, h, ray.origin, lp, sp);
			
				p += trace(scene, ray, max_depth) * inv_samples;
			}
			output.set_color(w, h, p);
        }
    }
	if(!output.save(scene.output))
		std::cerr << "Failed to save output file: " << scene.output << std::endl;
}

Color Raytracer::trace(Scene &scene, const Ray &r, size_t depth, Object *excluded_obj) 
{
    bool inside = false;
	Intersection intersec;

    // if ray didn't intersect anything, return ambient color
    if(!intersection(scene, r, intersec, NULL, excluded_obj, &inside))
        return scene.ambient.color;

	Material mat = intersec.object.material;

    Color amb_clr = scene.ambient.color * mat.kA;
    Color diff_clr;
    Color spec_clr;

    // try reach every light source
    for(std::vector<Light>::iterator it = scene.lights.begin(); it != scene.lights.end(); ++it) 
	{
        // light direction
		Light lt = *it;
		float inv_samples = 1.0f/lt.num_samples;
		for( int j = 0; j < lt.num_samples; j++) 
		{
			Point light_pos = lt.get_point();
			//Vector lightDir = (it->pos - intersec.contact).normalize();
			Vector lightDir = (light_pos - intersec.contact).normalize();

			// light source distance.
			//float dist = Point::distance(it->pos, intersec.contact);
			float dist = Point::distance(light_pos, intersec.contact);

			// attenuation.
			float att = 1.0f / (it->att.a + dist * it->att.b + (dist * dist) * it->att.c);

			Intersection useless;
			Ray rl(intersec.contact, lightDir);
			//if ray didn't intersects any object, it reaches the light
			//if(!intersection(scene, rl, useless, &it->pos, &intersec.object)) 
			if(!intersection(scene, rl, useless, &it->pos, &intersec.object)) 
			{
				// diffuse light.
				float cosDiff = SATURATE(Vector::dot(lightDir, intersec.normal));
				//diff_clr += it->color * cosDiff * mat.kD * att;
				diff_clr += (lt.color * cosDiff * mat.kD * att) * inv_samples;

				// Specular light.
				// halfway vector.
				Vector e = r.direction * (-1); 
				Vector halfway = lightDir + e;
				halfway.normalize();

				// cos of the angle between the halfway vector and the normal.
				float cosSpec = SATURATE(Vector::dot(halfway, intersec.normal.normalize()));

				// Shininess.
				cosSpec = std::pow(cosSpec, mat.alpha);
				//spec_clr += it->color * cosSpec * mat.kS * att;
				spec_clr += (lt.color * cosSpec * mat.kS * att) * inv_samples;
			}
		}
    }

    Color reflect_clr;
    Color refract_clr;

    // just keep shoting ray, if there is depth
    if(depth > 0) 
	{
		if(mat.kR > 0)
		{
			// reflected component added in recursively.
			Vector reflect_dir = get_reflection_direction(r.direction, intersec.normal);
			Ray reflec_ray(intersec.contact, reflect_dir);
			reflect_clr += trace(scene, reflec_ray, depth - 1, &intersec.object) * mat.kR;
		}


		if(mat.kT > 0)
		{
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
    }

	Color final_color = (intersec.color * (amb_clr + diff_clr) + spec_clr + reflect_clr + refract_clr);
	final_color.clamp();
	return final_color;
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

		//if found closest until now, save info
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
		intersection.contact = ray.origin + closest_t * ray.direction;
		intersection.object = *closest_obj;
		intersection.normal = closest_normal;
		intersection.color = intersection.object.get_color(intersection.contact);
		
		// check if camera is inside hit sphere.
		if(intersection.object.type == ObjectType::Sphere) 
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
    float d = Vector::dot((-1) * dir, normal);
    return (dir + (2 * normal * d)).normalize();
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