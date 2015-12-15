A free-dependent c++ raytracing project coded as an assignment of a computer graphics course. 

Coded and tested using Visual Studio C++ 2012 on Windows 10 and G++ on Ubuntu 14.04.

### How do I get set up? ###

On Windows:

Open Visual Studio solution ( ./raytracing.sln ) and compile.

On Linux:

Open terminal, travels to root folder and run compile.sh (./compile.sh)

### Running ###

It works in command line. Call the program name passing four parameters
input file that describes the scene.
output file it is the raytraced image (it just save as ppm file).
image width
image height

for example:
On Windows:
Raytracing.exe input.in output.ppm 800 600

On Linux:
./Raytracing input.in output.ppm 800 600

### Features ###

Multi-sampling using Multi-jittering.
Collision with spheres, planes, torus and cylinders.
Support to three kinds of texture (solid, checker and image(.ppm)).
Simulate lens aperture and depth of field.
Support to area lights (plane, spheres, hemispheres and circles).
Rigid transformations


### Input file ###

In order to configure a 3D scene, it must provide an text file as follows:

First four lines describe the camera:
- CameraCenterPosition (3 floats)
- CameraLookAtPosition (3 floats)
- CameraVectorUp (3 floats)
- FOVy (1 float) -NumSamplesPerPixel (1 int) -RadiusLens (1 float) -FocalLength (1 float) -ShutterSpeed (1 int) -ExposureValue (1 int)

A integer value informing number of light sources.
- NumberOfLightSources (1 int)
First light is used as ambient
- Position (3 floats) -SpectrumRGB (3 floats) -OpenGLFallOff (3 floats)
Following lights
- Position (3 floats) -SpectrumRGB (3 floats) -OpenGLFallOff (3 floats) -TypeOfLightArea (string) -NumberOfSamplingPerLight (1 int) -LightAreaSize (1 float)

A integer value informing number of textures
- NumberOfTextures (1 int)
Following lines describe textures
- Type(string) following parameters vary depending of the texture type
- solid -SpectrumRGB (3 floats)
- checker -SpectrumRGB1 (3 floats) -SpectrumRGB2 (3 floats) -SquareSize (1 int)
- texmap -FileName(ppm image name) -VectorOfInterpolation1 (4 floats)  -VectorOfInterpolation2 (4 floats)  

A integer value informing number of materials
- NumberOfTextures (1 int)
Following lines describe materials in a phong style
- kAmbient (1 float) -kDiffuse (1 float) -kSpecular( 1 float) -kSpecularAlpha (1 int) -kReflection (1 float) -kTransmission (1 float) -kIndexOfRefraction(a.k.a n2) (1 float)  

A integer value informing number of objects
- NumberOfObjects (1 int)
Following lines describe objects
- index of texture (1 int) -index of material (1 int) -typeOfObject (string) -following parameters vary depending of the object type
- sphere -CenterPosition (3 floats) -Radius (1 float) -Scale (3 floats) -Acceleration (3 floats)
- cylinder -BottomZPosition (1 float) -TopZPosition (1 float) -Radius (1 float) -CenterPosition (3 floats) -Rotation (3 floats) - Acceleration (3 floats)
- torus -OutsideRadius (1 float) -InsideRadius (1 float)  -CenterPosition (3 floats) -Rotation (3 floats) - Acceleration (3 floats)
- polyhedron -NumberOfPlanes (1 int) 
	For each polyhedron plane -NormalVector (3 floats) -Distance (1 float) 
	
Input file example:
0    30   -200
0    10   -100
0     1      0
40 1 0.0 60 1 1
3
 0     0      0   1  1  1    1  0  0 
 60.0 160.0 -200.0   1  1  1    1  0  0 noarea 25 3
-22.31 50  -5.98   1  1  1    1  0  0 noarea 25 3
7
texmap       rainbow1.ppm
				0 .001   0   .12
                0    0   0   0
checker      .08  .25  .20     .93  .83  .82    40
solid        1    1    1
checker      .08  .25  .20     .93  .23  .22    20
solid        1    .2    .2
solid        0    .8    0
solid        0.1    .2    1
5
0.80 0.00 0.00     1   0.0  0   0
0.30 0.40 0.00     1   0  0   0
0.11 0.11 0.30  1000   0.7  0   0
0.11 0.61 0.30  50   0  0   0
0.1  0.1  0.8  400  0.0  0.9  1.5
11
0 0 sphere      0 0 0    600 1 1 1	0 0 0
2 4 sphere 		-25 0 -50 	20 	1 1 1	0 0 0
3 3 cylinder 	0 45 20  -94.66 5.66  44.66   0 25 -45 1 1 1	0 0 0
3 3 torus		20	5	44.61 -32.66  -44.61	0	0	20 	1 1 1		0 0 0
4 1 sphere     	60 60.66  105.61     20  1.5 1 1.5		0 0 0
1 1 polyhedron 1
                 0  1  0    60
                 1  0  0  -300
                -1  0  0  -300
                 0  0 -1  -300
                 0  0  1  -300
