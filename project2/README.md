This is the code for ECS275 Project 2, which is modified from previous Project 1.

In this homework, anti-aliasing, depth of field and motion blur effects are added to the original recursive ray-tracing program.

To compile and run the code, use "cmake ." to generate the Makefile, "make" to compile and then "./specter [testscene filename]" to run the code.

Algorithms in this project comes from the paper "Distributed Ray Tracing" by Robert Cook. Algorithms in this project are mainly implemented in render() function in scene.cc.

8 sample images are provided, which are:
- no_effects.ppm: The image that three effects are all off. Specular refelection is still on, there's no specular refelction result on the front blue sphere just because it's actually closer to the eye and far away from other objects.
- anti-aliasing.ppm: The image that only anti-aliasing is on. Samples per pixel: 100.
- depth_of_field.ppm: The image that only depth of field is on. The distance between image and lens is set to a value that eye can focus on the blue sphere. It can be observed that the triangle and spheres behind are a little vague. Samples per pixel: 100.
- motion_blur.ppm: The image that only motion blur is on. It can be observed that the objects in the scene are moving upward. Samples per pixel: 100.
- anti_aliasing_and_depth_of_field.ppm: The image that anti-aliasing and depth of field are on. Samples per pixel: 100.
- anti_aliasing_and_motion_blur: The image that anti-aliasing and motion blur are on. Samples per pixel: 100.
- depth_of_field_and_motion_blur: The image that anti-aliasing and motion blur are on. Samples per pixel: 100.
- all_three_effects.ppm: All three effects are on. Samples per pixel: 100.


Anti-aliasing:
- To have effects of anti-aliasing, change control flag "EnableAntiAliasing" in test scene file to 1. To disable anti-aliasing, change it to 0.
- To control the number of samples, modify the number after "sample" in test scene file.
- Anti-aliasing is achieved by sampling on the pixel by randomly chosing sample number of sub-pixels in the range of (i:i+1, j:j+1), and then produe the ray to trace using this pixel.

Depth of field:
- To have effects of dept of field, change control flag "EnableDepthofField" in test scene file to 1. To disable depth of field, change it to 0.
- To control the number of samples, modify the number after "sample" in test scene file.
- To control the depth of field effect, modify the number following "F", which means focal length of lens; "D", distance from lens center to image; and "n", aperture number.
- Depth of field is achieved by sampling on the lens, which actually is placed in the eye in pinhole camera model.
- Like how human eye works, the image sensor is actually behind human eyes. Having D which is the distance between eye and image sensor, focal plane can be calculated easily.
- Calculate the intersect point Q between previously generated ray to trace and the focal plane(which corresponds to each pixel or sub-pixel on the image).
- Randomly choose a point L on the lens, and do recursive ray tracing on the ray from the point L to Q. After taking average on sampling points, the depth of field effect is produced.

Motion blur:
- To have effects of motion blur, change control flag "EnableMotionBlur" in test scene file to 1. To disable motion blur, change it to 0.
- To control the motion blur effect, modify the vector after "motion", which is the motion for all objects in the scene in a time period [0:1].
- Motion blur is achieved by sampling on the time period [0:1], that randomly choose a time between 0 and 1, set the objects in the scene at the position at this time, and do the ray tracing.

In fact, the choices of sampling point(time) are not that random. For anti-aliasing and depth of field, chosen points are restricted inside subpixel blocks where a pixel/whole lens has sample number of subpixels/blocks. For motion blur, every sampling time is in the randomly permuted time intervals, which, well, not easy to describe. Please refer to the code for details.
