
#include "SpecularMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "AreaLight.h"
#include "PointLight.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
#include "Math.h"
#include <cstdlib>
#include <iostream>
using namespace std;

SpecularMaterial::SpecularMaterial(const Color& color, float Kd, float Ka, float Ks)
  :color(color), Kd(Kd), Ka(Ka), Ks(Ks)
{
}

SpecularMaterial::~SpecularMaterial()
{
}

Color SpecularMaterial::getcolor() const {
	return color;
}

double SpecularMaterial::getKd() const {
	return Kd;
}
void SpecularMaterial::directIllumination(Color& result, const Ray& ray, const HitRecord& hit, const RenderContext& context) const
{
	int n = 1;
	const Scene* scene = context.getScene();
	const vector<AreaLight*>& lights = scene->getLights();
	Point x = ray.origin() + ray.direction()*hit.minT();
	Vector normal;
	hit.getPrimitive()->normal(normal, context, x, ray, hit);
	double costheta = Dot(normal, ray.direction());
	if(costheta > 0)
		normal = -normal;
	Vector theta = -ray.direction();
	const Object* world = scene->getObject();
	
    AreaLight*const* end = &lights[0]+lights.size();
	int pathnum = 100;
	Color res = Color(0, 0, 0);
	int size = lights.size();
	int iter = 0;
	//for (int i=0;i<pathnum;i++) {
		int k = rand()%size;
		Color light_color;
		Vector light_direction;
		AreaLight*const* begin = &lights[0];
		for (int j=0;j<k;j++) *begin++;
		double dist = (*begin)->getLight(light_color, light_direction, context, x);
		double cosphi = Dot(normal, light_direction);
		if (cosphi > 0) {
			HitRecord shadowhit(dist);
			Ray shadowray(x, light_direction);
			world->intersect(shadowhit, context, shadowray);
			if(!shadowhit.getPrimitive()) {
				// No shadows...
				//Vector theta_refl = -2 * Dot(normal, ray.direction()) * normal + ray.direction();
				Vector H = (theta + light_direction)/2;
				H.normalize();
				//double ksterm = Dot(light_direction, theta_refl);
				double ksterm = Dot(normal, H);
				double a = 1;
				for (int i=0;i<n;i++) a *= ksterm;
				double brdf = Kd + Ks * a;
				double radianceTransfer = cosphi * Dot(Vector(0, 0, -1.0), -light_direction) / (dist * dist);
				
				res += light_color*(200 * radianceTransfer * brdf);
			}
		}
	//}
	result = res;
  //result = res *(1 / (float)pathnum);
}

void SpecularMaterial::indirectIllumination(Color& result, const Ray& ray, const HitRecord& hit, const RenderContext& context) const
{
	int n = 1;
	const Scene* scene = context.getScene();
	Point x = ray.origin() + ray.direction()*hit.minT();
	Vector normal;
	hit.getPrimitive()->normal(normal, context, x, ray, hit);
	double costheta = Dot(normal, ray.direction());
	if(costheta > 0)
		normal = -normal;
	Vector theta = -ray.direction();
	const Object* world = scene->getObject();
	Color light = Color(0, 0, 0);
	
	double xxx = normal.x() * x.x() + normal.y() * x.y() + normal.z() * x.z();
	Point p1;
	if (normal.x() != 0) p1 = Point(xxx / normal.x(), 0, 0);
	else if (normal.y() != 0) p1 = Point(0, xxx / normal.y(), 0);
	else if (normal.z() != 0) p1 = Point(0, 0, xxx / normal.z());
	else p1 = Point(0, 0, 0);
	Vector v = p1 - x;
	v.normalize();
	Vector u = Cross(v, normal);
	u.normalize();
	
	double q = rand()%20000/20000.0;
	//double q1 = 3.141592653 * Kd;
	//double q2 = (2 * 3.141592653)/(n+1) * Ks;
	
	/*if (q < q1) {
		double uu = rand()%20000/20000.0;
		double theta1 = acos(sqrt(uu));
		double vv = rand()%20000/20000.0;
		double phi1 = 2 * 3.141592653 * vv;
		Vector sampleDir = normal * cos(theta1) + u * sin(theta1) * cos(phi1) + v * sin(theta1) * sin(phi1);
		HitRecord refl_hit(1000);
		Ray refl_ray(x, sampleDir);
		world->intersect(refl_hit, context, refl_ray);
		if (refl_hit.getPrimitive()) {
			const Material* matl = refl_hit.getMaterial();
			Color addon;
			Color atten(1,1,1);
			matl->shade(addon, context, refl_ray, refl_hit, atten, 0);
			light += addon * (Kd * Dot(normal, sampleDir) / q1);
		}
		else light += scene->getAmbient()*Ka;
	}
	else if (q < q1 + q2) {
		double uu = rand()%20000/20000.0;
		double theta1 = acos(pow(uu, 1.0/(n+1)));
		double vv = rand()%20000/20000.0;
		double phi1 = 2 * 3.141592653 * vv;
		Vector sampleDir = normal * cos(theta1) + u * sin(theta1) * cos(phi1) + v * sin(theta1) * sin(phi1);
		HitRecord refl_hit(1000);
		Ray refl_ray(x, sampleDir);
		world->intersect(refl_hit, context, refl_ray);
		if (refl_hit.getPrimitive()) {
			const Material* matl = refl_hit.getMaterial();
			Color addon;
			Color atten(1,1,1);
			matl->shade(addon, context, refl_ray, refl_hit, atten, 0);
			
			Vector H = (theta + sampleDir)/2;
			double ksterm = Dot(normal, H);
			double a = 1;
			for (int i=0;i<n;i++) a *= ksterm;
			double brdf = Ks * a;
			
			light += addon * (brdf * Dot(normal, sampleDir) / q2);
		}
		else light += scene->getAmbient()*Ka;
	}*/
	if (q < 0.7) {
		double uu = rand()%20000/20000.0;
		double theta1 = acos(uu);
		double vv = rand()%20000/20000.0;
		double phi1 = 2 * 3.141592653 * vv;
		Vector sampleDir = normal * cos(theta1) + u * sin(theta1) * cos(phi1) + v * sin(theta1) * sin(phi1);
		sampleDir.normalize();
		HitRecord refl_hit(1000);
		Ray refl_ray(x, sampleDir);
		world->intersect(refl_hit, context, refl_ray);
		if (refl_hit.getPrimitive()) {
			const Material* matl = refl_hit.getMaterial();
			Color addon;
			Color atten(1,1,1);
			matl->shade(addon, context, refl_ray, refl_hit, atten, 0);
			Vector H = (theta + sampleDir)/2;
			H.normalize();
			double ksterm = Dot(normal, H);
			double a = 1;
			for (int i=0;i<n;i++) a *= ksterm;
			double brdf = Ks * a + Kd;
			
			light += addon * (brdf * Dot(normal, sampleDir) / 0.7);
		}
		else light += scene->getAmbient()*Ka;
	}
	else light += Color(0, 0,0);
	
	result = light;
	//result = Color(0, 0,0);
}




void SpecularMaterial::shade(Color& result, const RenderContext& context,
                               const Ray& ray, const HitRecord& hit, const Color&, int) const
{
	int n = 1;
	const Scene* scene = context.getScene();
	const vector<PointLight*>& lights = scene->getnewLights();
	Point hitpos = ray.origin() + ray.direction()*hit.minT();
	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
	double costheta = Dot(normal, ray.direction());
	if(costheta > 0)
		normal = -normal;
	Vector theta = -ray.direction();
	const Object* world = scene->getObject();
	
	Color res = scene->getAmbient()*Ka;
	
	PointLight*const* begin = &lights[0];
    PointLight*const* end = &lights[0]+lights.size();
    while(begin != end){
		Color light_color;
		Vector light_direction;
		double dist = (*begin)->getLight(light_color, light_direction, context, hitpos);
		double cosphi = Dot(normal, light_direction);
		if(cosphi > 0){
			//
		// Cast shadow rays...
			HitRecord shadowhit(dist * 0.999);
			Ray shadowray(hitpos, light_direction);
			world->intersect(shadowhit, context, shadowray);
			if(!shadowhit.getPrimitive()) {
				//res = Color(1, 1, 1);
				// No shadows...
				//Vector theta_refl = -2 * Dot(normal, ray.direction()) * normal + ray.direction();
				Vector H = (theta + light_direction)/2;
				H.normalize();
				//double ksterm = Dot(light_direction, theta_refl);
				double ksterm = Dot(normal, H);
				double a = 1;
				for (int i=0;i<n;i++) a *= ksterm;
				double brdf = Kd + Ks * a;
				Vector lightn = (*begin)->normal;
				if (Dot(lightn, -light_direction) < 0) lightn = -lightn;
				double radianceTransfer = cosphi * Dot(lightn , -light_direction) / (dist * dist);
				
				res += light_color*((*begin)->radiance * radianceTransfer * brdf);
				//res = Color(1, 1, 1);
			}
			}
		*begin++;
		}
	result = res*color;
	//result = Color(0, 0, 0);
}
	/*
  const Scene* scene = context.getScene();
  const vector<Light*>& lights = scene->getLights();
  Point hitpos = ray.origin()+ray.direction()*hit.minT();
  Vector normal;
  hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
  double costheta = Dot(normal, ray.direction());
  if(costheta > 0)
    normal = -normal;

  const Object* world = scene->getObject();

  Color light = scene->getAmbient()*Ka;

  	HitRecord refl_hit(1000);
	Ray refl_ray(hitpos, -2 * Dot(normal, ray.direction()) * normal + ray.direction());
	world->intersect(refl_hit, context, refl_ray);
	if (refl_hit.getPrimitive()) {
		const Material* matl = refl_hit.getMaterial();
		Color addon;
		Color atten(1,1,1);
		matl->shade(addon, context, refl_ray, refl_hit, atten, 0);
		light += addon * Ks;
	}
  
#if 0
  for(vector<Light*>::const_iterator iter = lights.begin(); iter != lights.end(); iter++){
#else
    Light*const* begin = &lights[0];
    Light*const* end = &lights[0]+lights.size();
    while(begin != end){
#endif
    Color light_color;
    Vector light_direction;
    double dist = (*begin++)->getLight(light_color, light_direction, context, hitpos);
    double cosphi = Dot(normal, light_direction);
    if(cosphi > 0){
      // Cast shadow rays...
      HitRecord shadowhit(dist);
      Ray shadowray(hitpos, light_direction);
      world->intersect(shadowhit, context, shadowray);
      if(!shadowhit.getPrimitive())
        // No shadows...
        light += light_color*(Kd*cosphi);
	  }
    }
  result = light*color;
}*/
