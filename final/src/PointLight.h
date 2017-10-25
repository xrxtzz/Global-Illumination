
#ifndef PointLight_h
#define PointLight_h

#include "Light.h"
#include "Point.h"
#include "Color.h"
#include "Vector.h"

class PointLight : public Light {
 public:
  PointLight(const Point& position, const Color& color);
  virtual ~PointLight();

  virtual void preprocess();
  virtual double getLight(Color& light_color, Vector& light_direction,
                          const RenderContext& context, const Point& pos) const;

						  
  Point position;
  Color color;
  double radiance;
  Vector normal;  
 private:
  PointLight(const PointLight&);
  PointLight& operator=(const PointLight&);

};

#endif

