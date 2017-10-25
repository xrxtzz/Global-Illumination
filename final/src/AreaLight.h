
#ifndef AreaLight_h
#define AreaLight_h

#include "Light.h"
#include "Point.h"
#include "Color.h"
#include <cstdlib>

class AreaLight : public Light {
 public:
  AreaLight(const Point& position, const Color& color, const double& size);
  virtual ~AreaLight();

  virtual void preprocess();
  virtual double getLight(Color& light_color, Vector& light_direction,
                          const RenderContext& context, const Point& pos) const;
	Point position;
  Color color;
  double size;
 private:
  AreaLight(const AreaLight&);
  AreaLight& operator=(const AreaLight&);

  
};

#endif

