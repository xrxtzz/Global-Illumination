
#include "AreaLight.h"

AreaLight::AreaLight(const Point& position, const Color& color, const double& size)
  : position(position), color(color), size(size)
{
}

AreaLight::~AreaLight()
{
}

void AreaLight::preprocess()
{
}

double AreaLight::getLight(Color& light_color, Vector& light_direction,
                            const RenderContext&, const Point& hitpos) const
{
  light_color = color;
  double x = rand()%20000/20000.0 - 0.5;
  double y = rand()%20000/20000.0 - 0.5;
  Point samplePoint = Point(position.x() + x * size, position.y() + y * size, position.z());
  
  Vector dir = samplePoint-hitpos;
  double len = dir.normalize();
  light_direction = dir;
  return len;
}
