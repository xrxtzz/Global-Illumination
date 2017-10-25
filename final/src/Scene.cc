
#include "Scene.h"
#include "Background.h"
#include "Primitive.h"
#include "Camera.h"
#include "HitRecord.h"
#include "Image.h"
#include "Light.h"
#include "AreaLight.h"
#include "PointLight.h"
#include "Point.h"
#include "Material.h"
#include "Math.h"
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
    AreaLight* light = lights[i];
    light->preprocess();
  }
  double aspect_ratio = image->aspect_ratio();
  camera->preprocess(aspect_ratio);
  object->preprocess();
}

void Scene::trace_photon() {
	RenderContext context(this);
	AreaLight* light = lights[0];
	int trace_depth = 2;
	
	for (int k=0;k<photon_sample;k++) {
	Color light_color = light->color;
	Vector light_normal = Vector(0, 0, -1);
	Point pos;
	double radiance = 200;
	//randomly choose a point on area light source
	double x = rand()%20000/20000.0 - 0.5;
	double y = rand()%20000/20000.0 - 0.5;
	pos = Point(light->position.x() + x * light->size, light->position.y() + y * light->size, light->position.z());
	PointLight *thisone = new PointLight(pos, light_color);
	thisone->radiance = radiance;
	thisone->normal = light_normal;
	addnewLight(thisone);
	for (int i=0; i<trace_depth;i++) {
		//store this point light source and record light source
		
		//randomely & uniformly generate a direction
		double xxx = light_normal.x() * pos.x() + light_normal.y() * pos.y() + light_normal.z() * pos.z();
		Point p1;
		if (light_normal.x() != 0) p1 = Point(xxx / light_normal.x(), 0, 0);
		else if (light_normal.y() != 0) p1 = Point(0, xxx / light_normal.y(), 0);
		else if (light_normal.z() != 0) p1 = Point(0, 0, xxx / light_normal.z());
		else p1 = Point(0, 0, 0);
		Vector v = p1 - pos;
		v.normalize();
		Vector u = Cross(v, light_normal);
		u.normalize();
		
		double uu = rand()%20000/20000.0;
		double theta1 = acos(uu);
		double vv = rand()%20000/20000.0;
		double phi1 = 2 * 3.141592653 * vv;
		Vector sampleDir = light_normal * cos(theta1) + u * sin(theta1) * cos(phi1) + v * sin(theta1) * sin(phi1);
		sampleDir.normalize();
		//std::cout << sampleDir << std::endl;
		//trace that direction and create light sources, until reach some depth or hit nothing
		Ray ray(pos, sampleDir);
		HitRecord hit(DBL_MAX);
		object->intersect(hit, context, ray);
		if (hit.getPrimitive()) {
			//hit sth, update and go to next round
			const Material* matl = hit.getMaterial();
			Vector normal;
			hit.getPrimitive()->normal(normal, context, pos, ray, hit);
			double costheta = Dot(normal, ray.direction());
			if(costheta > 0)
				normal = -normal;
			radiance = 20 * (matl->getKd()/3.141592653 * radiance) * Dot(normal, -ray.direction());
			pos += sampleDir*hit.minT();
			light_normal = normal;
			light_color *= matl->getcolor();
			//std::cout << matl->getKd() << std::endl;
			PointLight *thisone = new PointLight(ray.origin() + ray.direction()*hit.minT(), light_color);
			thisone->radiance = radiance;
			thisone->normal = light_normal;
			addnewLight(thisone);
		}
		else break;
	}
	}
}


void Scene::render()
{
  if(!object || !background || !camera || !image){
    cerr << "Incomplete scene, cannot render!\n";
    exit(1);
  }
  
  trace_photon();
  std::cout << "Number of VPLs used:" << static_cast<int>(new_lights.size()) << std::endl;
  
  int xres = image->getXresolution();
  int yres = image->getYresolution();
  RenderContext context(this);
  double dx = 2./xres;
  double xmin = -1. + dx/2.;
  double dy = 2./yres;
  double ymin = -1. + dy/2.;
  Color atten(1,1,1);
  std::cout << "test";
  for(int i=0;i<yres;i++){
    //cerr << "y=" << i << '\n';
    double y = ymin + i*dy;
	for(int j=0;j<xres;j++){
		double x = xmin + j*dx;
		bool notdone = true;
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
					AreaLight*const* begin = &lights[0];
					AreaLight*const* end = &lights[0]+lights.size();
					Point x = ray.origin() + ray.direction()*hit.minT();
					
					while (begin != end) {
						Point pos = (*begin)->position;
						double size = (*begin)->size;
						if (pos.x()+size/2.0>x.x() && pos.x()-size/2.0<x.x() && pos.y()+size/2.0>x.y() && pos.y()-size/2.0<x.y() && pos.z()+0.1>x.z() && pos.z()-0.1<x.z()) {
							result = (*begin)->color;
							notdone = false;
						}
						*begin++;
					}
					if (notdone) {
						const Material* matl = hit.getMaterial();
						matl->shade(result, context, ray, hit, atten, 0);
					}
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
		if (notdone) finalres*=(1.0/photon_sample);
		
		
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
