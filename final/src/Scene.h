
#ifndef Scene_h
#define Scene_h

#include "Color.h"
#include "Vector.h"
#include <string>
#include <vector>

class Background;
class Camera;
class Color;
class Image;
class Light;
class Object;
class RenderContext;
class Ray;
class AreaLight;
class PointLight;

class Scene {
 public:
  Scene();
  virtual ~Scene();

  inline Object* getObject() const {
    return object;
  }
  void setObject(Object* obj) {
    object = obj;
  }

  inline Background* getBackground() const {
    return background;
  }
  void setBackground(Background* bg) {
    background = bg;
  }

  inline Camera* getCamera() const {
    return camera;
  }
  void setCamera(Camera* cam) {
    camera = cam;
  }

  inline Image* getImage() const {
    return image;
  }
  void setImage(Image* im) {
    image = im;
  }

  void addLight(AreaLight* light) {
    lights.push_back(light);
  }
  
  void addnewLight(PointLight *light) {
	  new_lights.push_back(light);
  }
  const std::vector<AreaLight*>& getLights() const {
    return lights;
  }
  
  const std::vector<PointLight*>& getnewLights() const {
    return new_lights;
  }

  Color getAmbient() const {
    return ambient;
  }
  void setAmbient(const Color& amb) {
    ambient = amb;
  }

  int getMaxRayDepth() const {
    return maxRayDepth;
  }
  void setMaxRayDepth(int rd) {
    maxRayDepth = rd;
  }
  
  double getMinAttenuation() const {
    return minAttenuation;
  }
  void setMinAttenuation(double atten) {
    minAttenuation = atten;
  }

  void setSample(int samp) {
	  sample = samp;
  }
  void setPhoton(int samp) {
	  photon_sample = samp;
  }
  void setF(double f) {
	  F = f;
  }
  void setD(double d) {
	  D = d;
  }
  void setn(int n1) {
	  n = n1;
  }
  void setMotion(Vector m) {
	  motion = m;
  }
  void controlAntiAliasing(int a) {
	  if (a>0) do_anti_aliasing = true;
	  else do_anti_aliasing = false;
  }
  void controlDepthofField(int a) {
	  if (a > 0) do_depth_of_field = true;
	  else do_depth_of_field = false;
  }
  void controlMotionBlur(int a) {
	  if (a > 0) do_motion_blur = true;
	  else do_motion_blur = false;
  }
  
  void preprocess();
  void render();
  void trace_photon();
  double traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& attenuation, int depth) const;
  double traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& attenuation, int depth) const;

 private:
  Scene(const Scene&);
  Scene& operator=(const Scene&);

  Background* background;
  Camera* camera;
  Color ambient;
  Image* image;
  Object* object;
  std::vector<AreaLight*> lights;
  std::vector<PointLight*> new_lights;
  int maxRayDepth;
  double minAttenuation;
  int sample;
  int photon_sample;
  double F, D, n;
  Vector motion;
  bool do_anti_aliasing;
  bool do_depth_of_field;
  bool do_motion_blur;
};

#endif
