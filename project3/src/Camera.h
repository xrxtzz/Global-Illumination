
#ifndef Camera_h
#define Camera_h

class Ray;
class RenderContext;
class Point;

class Camera {
 public:
  Camera();
  virtual ~Camera();

  virtual void preprocess(double aspect_ratio) = 0;
  virtual void makeRay(Ray& ray, const RenderContext& context, double x, double y) const = 0;
  virtual void newmakeRay(Ray& ray, const RenderContext& context, double x, double y, double F, double V, double theta, double dis) const = 0;
  
 private:
  Camera(const Camera&);
  Camera& operator=(const Camera&);
};

#endif

