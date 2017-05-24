
#include "Scene.h"
#include "Background.h"
#include "Camera.h"
#include "HitRecord.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "Ray.h"
#include "RenderContext.h"
#include "PinholeCamera.h"
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
using namespace std;

Scene::Scene()
{
  object = 0;
  background = 0;
  camera = 0;
  ambient = Color(0, 0, 0);
  image = 0;
  minAttenuation = 0;
  do_anti_aliasing = false;
  do_motion_blur = false;
  do_depth_of_field = false;
}

Scene::~Scene()
{
  delete object;
  delete background;
  delete camera;
  delete image;
  for(int i=0;i<static_cast<int>(lights.size());i++){
    Light* light = lights[i];
    delete light;
  }
}

void Scene::preprocess()
{
  background->preprocess();
  for(int i=0;i<static_cast<int>(lights.size());i++){
    Light* light = lights[i];
    light->preprocess();
  }
  double aspect_ratio = image->aspect_ratio();
  camera->preprocess(aspect_ratio);
  object->preprocess();
}

void Scene::render()
{
  if(!object || !background || !camera || !image){
    cerr << "Incomplete scene, cannot render!\n";
    exit(1);
  }
  int xres = image->getXresolution();
  int yres = image->getYresolution();
  RenderContext context(this);
  double dx = 2./xres;
  double xmin = -1. + dx/2.;
  double dy = 2./yres;
  double ymin = -1. + dy/2.;
  Color atten(1,1,1);
  for(int i=0;i<yres;i++){
    //cerr << "y=" << i << '\n';
    double y = ymin + i*dy;
	for(int j=0;j<xres;j++){
		double x = xmin + j*dx;
		//cerr << "x=" << j << ", y=" << i << '\n';
		Color finalres = Color(0, 0, 0);
		//generate random permuation
		int *perm;
		perm = new int[sample];
		for (int ii=0;ii<sample;ii++) perm[ii] = ii;
		for (int ii=0;ii<sample-1;ii++) {
			int jj = rand()%(sample - ii)+ii;
			int t = perm[ii];
			perm[ii] = perm[jj];
			perm[jj] = t;
		}
		
		for (int k=0;k<sample;k++) {
			
			
			int kk = floor(sqrt(sample));
			//SAMPLING HERE
			//anti-aliasing, sample on x and y
			double xx, yy;
			if (do_anti_aliasing) {
				xx = (rand()%20000/20000.0);
				yy = (rand()%20000/20000.0);
				xx += (k%kk);
				xx /= (double)kk;
				yy += (k/kk);
				yy /= (double)kk;
				xx *= dx; yy *= dy;
				x+=xx; y+=yy;
			}
			//depth of field, sample on lens given by F, D(distance to eye), n
			Ray ray;
			if (do_depth_of_field) {
				double theta = (rand()%20000/20000.0);
				double dis = rand()%20000/20000.0;
				theta += (k%kk);
				theta /= (double)kk;
				dis += (k/kk);
				dis /= (double)kk;
				theta *= 2 * 3.141592653;
				dis *= 0.5 * F/n;
				camera->newmakeRay(ray, context, x, y, F, D, theta, dis);
			}
			else camera->makeRay(ray, context, x, y);
			
			//motion blur, sample on time period [0, 1], the motion is defined in motion vector, that describes the motion on objects
			Vector movet;
			if (do_motion_blur) {
				double t = rand()%20000/20000.0;
				t = (t+perm[k])/sample;
				movet = motion * t;
				context.scene->object->move(movet);
			}
			
			//recursive ray tracing
			HitRecord hit(DBL_MAX);
			Color result;
			object->intersect(hit, context, ray);
				if(hit.getPrimitive()){
				// Ray hit something...
				const Material* matl = hit.getMaterial();
				matl->shade(result, context, ray, hit, atten, 0);
			} else {
				background->getBackgroundColor(result, context, ray);
			}
			
			//Recover..
			if (do_anti_aliasing) {
				x-=xx; y-=yy;
			}
			if (do_motion_blur)
				context.scene->object->move(-movet);
			
			finalres = finalres+result;
		}
		finalres*=(1.0/sample);
		image->set(j, i, finalres);
	}
  }
}

double Scene::traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& atten, int depth) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    object->intersect(hit, context, ray);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}

double Scene::traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& atten, int depth) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    obj->intersect(hit, context, ray);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}
