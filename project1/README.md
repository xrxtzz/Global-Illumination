# Project 1

This is the code for ECS275 Project 1, which is modified from provided reference code. 
In addition to originally implemented rendering scene that includes spheres and planes, it can now render triangles thus also polygons. Also specular reflection is added.
To compile and run the code, use "cmake ." to generate Makefile, "make" to compile and then "./specter [testscene filename]" to run the code. You may need to delete CMakeCache.txt first before cmake.

Modification on original reference includes: 
 - New files Triangle.h and Triangle.cc file that add and implement the class Triangle, which is also a subclass of Primitive, similar to Sphere and Plane.
 - Modification on Parser.cc that it can reads triangle information.
 - Modification on LambertianMaterial.cc that adds specular reflection(recursive ray-tracing)
	
The acceptable scene file has similar form with origninal scene file, which now also accepts triangle information at scene.
To input a triangle, it requires the material information along with position information, includes normal(which does not really works since actual normal will be calculated when creating instances of triangle objects) and positions of three points.
The output file is in .ppm format that can be open in linux "eog" command.
A sample scene file "test.scn" and its generated rendered image "image.ppm" are provided. The image can also be obtained by "./specter test.scn" after compilation.

There's actually some mistakes in the code. Firstly I should NOT add the specular reflection algorithm directly in LambertianMaterial.cc because lambertian material actually means a kind of material that can perfectly diffuse comming lights. Secondly the BRDF function I used in specular reflection is not correct. I did not use any model(like phong model) and simply used Kd as the reflectance term. These mistakes are corrected in Project 3, where I added a new class SpecularMaterial to do all the stuff and reverted the shading function in LambertianMaterial.