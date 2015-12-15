A free-dependent c++ raytracing project coded as an assignment of a computer graphics course. 

Coded and testes using Visual Studio C++ 2012 on Windows 10 and G++ on Ubuntu 14.04.

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