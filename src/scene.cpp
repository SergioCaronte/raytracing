#include "scene.h"
#include "raytracer.h"
#include "math/plane.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

void Scene::load_file(int argc, char **argv) 
{
	if(argc < 3)
	{
		std::cerr << "Cmd line usage: " << argv[0] << " input output [width] [height]\n";
        exit(1);
	}

	// input file name
	input = argv[1];
	// output file name
    output = argv[2];

	// check if resolution parameters were set
    if(argc == 5) 
	{
        screen.width_px = strtoul(argv[3], NULL, 10);
        screen.height_px = strtoul(argv[4], NULL, 10);
    }
    else
	{
        screen.width_px = 800;
        screen.height_px = 600;
    }

    // open the input file.
    std::ifstream f_input(input);
    if(!f_input.is_open())
        printf("Failed to open input file (%s).", input);

    // parse the input file.
    parse_camera(f_input);
    parse_light(f_input);
    parse_texture(f_input);
    parse_material(f_input);
    parse_object(f_input);

    // close the input file.
    f_input.close();
}

void Scene::calculate_screen() 
{
	//screen.aspect =  (float)screen.width_px/ (float)screen.height_px;

	//screen.samples = 4;
 //   screen.d = Point::distance(screen.center, camera.pos);

 //   // Sizes
 //   screen.height = 2 * tan(toRads(camera.fovy / 2.0f)) * screen.d;
 //   screen.width = (screen.height * screen.width_px) / screen.height_px;

 //   // Directions
 //   screen.right_dir = Vector::cross(camera.dir, camera.up);
 //   screen.right_dir.normalize();
 //   screen.bottom_dir = camera.up * (-1);
 //   screen.bottom_dir.normalize();

 //   // Top left pixel.
 //   screen.top_left = screen.center;
 //   screen.top_left -= screen.right_dir * (screen.width / 2.0f);
 //   screen.top_left += camera.up * (screen.height / 2.0f);

 //   // Pixel size.
 //   screen.px_size_w = screen.width / (float) screen.width_px;
 //   screen.px_size_h = screen.height / (float) screen.height_px;
}

void Scene::calculate_uvw()
{
	Vector view = camera.lookat - camera.pos;
	view.normalize();
	camera.z = view * (-1);
	camera.x = Vector::cross(view, camera.up);
	camera.x.normalize();
	camera.y = Vector::cross(camera.z, camera.x);
	camera.y.normalize();
}

void Scene::parse_camera(std::ifstream &in) 
{
    in >> camera.pos.x >> camera.pos.y >> camera.pos.z;

    in >> screen.center.x >> screen.center.y >> screen.center.z;
	camera.lookat = screen.center;
    camera.dir = screen.center - camera.pos;
    camera.dir.normalize();

    in >> camera.up.x >> camera.up.y >> camera.up.z;
    camera.up.normalize();

	in >> camera.fovy >> screen.samples >> camera.lens_radius >> camera.focal_dist >> camera.exposure_time;
	
	// calculate_screen();
	calculate_uvw();

	// screen features
	screen.theta = M_PI * camera.fovy/180;
	Vector dist = camera.pos - camera.lookat;

	// calculate viewplane distance
	screen.d = 0.5f * screen.height_px  / tan((M_PI/180.0)*0.5f*camera.fovy);
	
	// add viewplane distance to focal_distance	
	camera.focal_dist += Point::distance(camera.pos, camera.lookat);
	
	//useless
	screen.aspect =  (float)screen.width_px/ (float)screen.height_px;
	screen.px_size_h = 2 * tan(screen.theta/2.0);
	screen.px_size_w = screen.px_size_h * screen.aspect;

	// start camera sampler
	camera.sampler = new MultiJittered(screen.samples);
	camera.sampler->map_samples_to_unit_disk();

}

void Scene::parse_light(std::ifstream &in) 
{
    Light light;

    in >> numLights;
    if(in.fail())
        printf("Failed to read the input file (light description)");

    // ambient light.
    in >> ambient.pos.x >> ambient.pos.y >> ambient.pos.z;
    in >> ambient.color.r >> ambient.color.g >> ambient.color.b;
    in >> ambient.att.a >> ambient.att.b >> ambient.att.c;

    // other lights.
	std::string type;
    for(size_t i = 1; i < numLights; ++i) 
	{
        in >> light.pos.x >> light.pos.y >> light.pos.z;
        in >> light.color.r >> light.color.g >> light.color.b;
        in >> light.att.a >> light.att.b >> light.att.c;
		in >> type;
		in >> light.num_samples >> light.area_size;
		
		light.build_area(type);
        lights.push_back(light);
    }
}

void Scene::parse_texture(std::ifstream &in) 
{
    std::string type;
    Texture tex;

    in >> numTextures;
    if(in.fail())
        printf("Failed to read the input file (texture description).");

    for(size_t i = 0; i < numTextures; ++i) 
	{
        tex.id = i;
        in >> type;

        if(type == "solid") 
		{
            tex.type = SolidTexture;
            in >> tex.solid.color.r >> tex.solid.color.g >> tex.solid.color.b;
        }
        else if(type == "checker") 
		{
            tex.type = CheckerTexture;
            in >> tex.checker.color1.r >> tex.checker.color1.g
                >> tex.checker.color1.b;
            in >> tex.checker.color2.r >> tex.checker.color2.g
                >> tex.checker.color2.b;
            in >> tex.checker.scale;

            tex.checker.map[0][0] = tex.checker.color1;
            tex.checker.map[0][1] = tex.checker.color2;
            tex.checker.map[1][0] = tex.checker.color1;
            tex.checker.map[1][1] = tex.checker.color2;
        }
        else if(type == "texmap") 
		{
            tex.type = MapTexture;
            in >> tex.map.filename;
            in >> tex.map.p0.x >> tex.map.p0.y >> tex.map.p0.z >> tex.map.p0.w;
            in >> tex.map.p1.x >> tex.map.p1.y >> tex.map.p1.z >> tex.map.p1.w;
            tex.map.image.load(tex.map.filename);
        }
        else 
		{
            printf("invalid texture type (%s).", type.c_str());
        }
        textures.push_back(tex);
    }
}

void Scene::parse_material(std::istream &in) 
{
    Material material;

    in >> numMaterials;
    if(in.fail())
        printf("Failed to read the input file (material description).");

    for(size_t i = 0; i < numMaterials; ++i) 
	{
        material.id = i;
        in >> material.kA >> material.kD >> material.kS >> material.alpha
           >> material.kR >> material.kT >> material.ior;
        materials.push_back(material);
    }
}

void Scene::parse_object(std::istream &in) 
{
    std::string type;

    in >> numObjects;
    if(in.fail())
        printf("Failed to read the input file (object description).");

    for(size_t i = 0; i < numObjects; ++i) 
	{
        int texId, matId;
		in >> texId >> matId;
        in >> type;

        if(type == "sphere") 
		{
			SphereObject* object = new SphereObject();
			object->id = i;
			object->texture = textures[texId];
			object->material = materials[matId];

            in >> object->pos.x >> object->pos.y >> object->pos.z;
            in >> object->radius;
			objects.push_back(object);
        }
        else if(type == "polyhedron") 
		{
			PolyhedronObject* object = new PolyhedronObject();
			object->id = i;
			object->texture = textures[texId];
			object->material = materials[matId];

            in >> object->numFaces;
            if(in.fail())
                printf("Failed to read the input file (polyhedron description).");

            Plane plane;
            for(size_t j = 0; j < object->numFaces; ++j) 
			{
                in >> plane.a >> plane.b >> plane.c >> plane.d;
                object->planes.push_back(plane);
            }
			objects.push_back(object);
        }
		else if(type == "torus") 
		{
			TorusObject* object = new TorusObject();
			object->id = i;
			object->texture = textures[texId];
			object->material = materials[matId];

			in >> object->radius >> object->thickness;
			in >> object->pos.x >> object->pos.y >> object->pos.z;
			in >> object->rot.x >> object->rot.y >> object->rot.z;
			object->calculate_matrices();
			objects.push_back(object);
		}
		else if(type == "cylinder")
		{
			CylinderObject* object = new CylinderObject();
			object->id = i;
			object->texture = textures[texId];
			object->material = materials[matId];

			in >> object->bottom >> object->top >> object->radius;
			in >> object->pos.x >> object->pos.y >> object->pos.z;
			in >> object->rot.x >> object->rot.y >> object->rot.z;
			object->calculate_matrices();
			objects.push_back(object);
		}
        else 
		{
            printf("invalid object type(%s).", type.c_str());
        }
    }
}


