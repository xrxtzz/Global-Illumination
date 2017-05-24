
#ifndef Triangle_h
#define Triangle_h

#include "Vector.h"
#include "Primitive.h"
class HitRecord;
class Point;
class RenderContext;

class Triangle : public Primitive {
 public:
  Triangle(Material* material, const Vector& n, const Point& point1, const Point& point2, const Point& point3);
  virtual ~Triangle();

  virtual void getBounds(BoundingBox& bbox) const;
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point & hitpos, const Ray& ray, const HitRecord& hit) const;
  virtual void move(const Vector& motion);

 private:
  Triangle(const Triangle&);
  Triangle& operator=(const Triangle&);

  Vector n;
  double d;
  Point point1, point2, point3;
};

#endif

