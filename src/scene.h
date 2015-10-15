#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include "structs.h"

class Scene 
{
public:
	Scene(){}

	void load_file(int argc, char **argv);
	void compute();
    
	char *input;
    char *output;

    Screen screen;
    Camera camera;

    size_t numLights;
    Light ambient;
    std::vector<Light> lights;

    size_t numTextures;
    std::vector<Texture> textures;

	size_t numMaterials;
	std::vector<Material> materials;

    size_t numObjects;
	std::vector<Object*> objects;

protected:
	void calculate_base();
	void parse_camera(std::ifstream &in);
	void parse_light(std::ifstream &in);
	void parse_texture(std::ifstream &in);
	void parse_material(std::istream &in);
	void parse_object(std::istream &in);
};
#endif 
