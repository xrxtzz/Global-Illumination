# Project 3

This is the code for ECS275 Project 3, which is modified from previous Project 2.

In this homework, global path tracing algorithm is implemented, which includes direct illumination from area light sources and indirect illumination from context reflections.

This brings the penumbra effect which comes from sampling on area light sources, and color shading effect which comes from sampling on diffuse reflections of neighboring surfaces.

To compile and run the code, use "cmake ." to generate the Makefile, and "make" to compile, and then "./specter [testscene filename]" to render the scene. You may need to delete the CMakeCache.txt before cmake. The algorithms are implemented in SpecularMaterial.cc file.

5 sample images are provided. These images are:
- sample1.ppm: A white sphere in the center of a bounding box where left side is a red wall and right side is a blue wall. There's one light source in the scene, which is right above the whole bounding box. Penumbra effects can be observed on the ground. Color shading can be observed on two sides of the sphere, where there's some red shading on the left side and blue shading on the right side. # of samples per pixel: 625. Anti-aliasing is also included.
- sample2.ppm: This sample includes the same bounding box as sample1.ppm. It contains 3 spheres, including a white sphere like sample1.ppm, a cyan sphere floating in the up-left-behind side of the white sphere, and a yellow sphere floating in the up-right-front side of the white sphere. In addition to the penumbra effects and color shading effects which are same as previous one, it can be observed that, slightly, there's some yellow shading on the white sphere reflected from the yellow sphere. Also on the cyan sphere, the color is brighter on the side facing the white sphere, which comes from diffuse reflection of the white sphere. # of samples per pixel: 100. Anti-aliasing is also included.
- sample3.ppm: This sample describes a same scene as sample2.ppm, but it has more samples. Effects described above in sample2.ppm can be observed more clearly. # of samples per pixel: 625. Anti-aliasing is also included.
- sample4.ppm: This sample contains the same bounding box and spheres as sample2.ppm and sample3.ppm, but it has 2 area light sources. These two light sources are al floating above the whole scene, one is on the left-front side of the scene and the other is on the right-behind side of the scene. Penumbra effects on the ground and walls can be viewed more clearly. # of samples per pixel: 100. Anti-aliasing is also included.
- sample5.ppm: This sample describes a same scene as sample4.ppm, but it has more samples. Effects described above in sample4.ppm can be observed more clearly. # of samples per pixel: 625. Anti-aliasing is also included.


Direct illumination:
- Direct illumination is implemented by sampling on area light sources. For convinience, area light sources in this project are assigned to a constant normal. User can define the size of light sources in the test scene file.
- Every time when calculating the direct illumination for the tracing ray, there's only one random ray between area light source and the hit point. And sampling is implemented outside of the ray-tracing function, which makes the whole rendering process accelerated(because direct illumination will also be called recursively) and combining this effect with anti-aliasing easily.
- BRDF used in the direct illumination comes from the modifed Blinn-Phong model.

Indirect illumination:
- Indirect illumination is implemented by sampling on the unit hemisphere.
- Like direct illumination, every time there's only one diffuse reflection ray is traced.
- BRDF used in the indrect illumination comes from the modifed Blinn-Phong model.
- Russian Roulette is used. Probability in Russian Roulette is set to 0.7.

I have to say I was lazy at finding proper scene parameters that Kd actually should be less than 1/Pi and the probability in Russian Roulette should follow some formular derived from Kd and Ks and n. Also I did not let n in modified Blinn-Phong model and emitted radiance of light sourcesbe a value that can modified by user. If you wanna see the effect of different n and radiance, please go to SpecularMaterial.cc and compile again.

One other words, the algorithms are running quite slow...It tooks like 1 minite to generate a 512*512 image with 25 samples on my cheap Vultr VPS and 6 minites to render an image with 100 samples, which is, still not good. It requires like half an hour to render an image with 625 samples.