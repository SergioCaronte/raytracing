#include "scene.h"
#include "raytracer.h"

int main(int argc, char **argv) {
    Scene scene;
	scene.load_file(argc, argv);
    
	Raytracer rt;
	rt.compute(scene);

	system("pause");

    return 0;
}
