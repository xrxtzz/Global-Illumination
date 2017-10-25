
#include "LambertianMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "AreaLight.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
#include "Math.h"
using namespace std;

LambertianMaterial::LambertianMaterial(const Color& color, float Kd, float Ka)
  :color(color), Kd(Kd), Ka(Ka)
{
}

LambertianMaterial::~LambertianMaterial()
{
}

Color LambertianMaterial::getcolor() const {
	return color;
}

double LambertianMaterial::getKd() const {
	return Kd;
}

void LambertianMaterial::shade(Color& result, const RenderContext& context,
                               const Ray& ray, const HitRecord& hit, const Color&, int) const
{
  const Scene* scene = context.getScene();
  const vector<AreaLight*>& lights = scene->getLights();
  Point hitpos = ray.origin()+ray.direction()*hit.minT();
  Vector normal;
  hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
  double costheta = Dot(normal, ray.direction());
  if(costheta > 0)
    normal = -normal;

  const Object* world = scene->getObject();

  Color light = scene->getAmbient()*Ka;
  
#if 0
  for(vector<AreaLight*>::const_iterator iter = lights.begin(); iter != lights.end(); iter++){
#else
    AreaLight*const* begin = &lights[0];
    AreaLight*const* end = &lights[0]+lights.size();
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
}
