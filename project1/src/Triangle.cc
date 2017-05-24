
#include "Triangle.h"
#include "HitRecord.h"
#include "Math.h"
#include "Ray.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

Triangle::Triangle(Material* material, const Vector& normal, const Point& point1, const Point& point2, const Point& point3)
  : Primitive(material), n(normal), point1(point1), point2(point2), point3(point3)
{
	Vector N = Cross(point1 - point2, point2 - point3);
	n = N;
  n.normalize();
  d = Dot(n, point1);
}

Triangle::~Triangle()
{
}

void Triangle::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
  double denom = Dot(n, ray.direction());
  Vector qp1(ray.origin() - point1);
  Vector qp2(ray.origin() - point2);
  Vector qp3(ray.origin() - point3);
  Vector N1(Cross(qp1, qp2));
  Vector N2(Cross(qp2, qp3));
  Vector N3(Cross(qp3, qp1));
  
  if(Abs(denom) > 1.e-6 && denom*Dot(ray.direction(), N1) > 0 && denom*Dot(ray.direction(), N2) > 0 && denom*Dot(ray.direction(), N3) > 0){
    double t = (d-Dot(n, ray.origin()))/denom;
    hit.hit(t, this, matl);
  }
}

void Triangle::normal(Vector& normal, const RenderContext&,
                   const Point&, const Ray&, const HitRecord&) const
{
  normal = n;
}

void Triangle::getBounds(BoundingBox& bbox) const
{
  cerr <<  "Plane::getBounds() called!" << endl;
  exit( 1 );
}

