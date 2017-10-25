
#ifndef SpecularMaterial_h
#define SpecularMaterial_h

#include "Material.h"
#include "Color.h"

class SpecularMaterial : public Material {
 public:
  SpecularMaterial(const Color& color, float Kd, float Ka, float Ks);
  virtual ~SpecularMaterial();

  virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const;
  virtual void directIllumination(Color& result, const Ray& ray, const HitRecord& hit, const RenderContext& context) const;
  virtual void indirectIllumination(Color & result, const Ray& ray, const HitRecord& hit, const RenderContext& context)const;
   virtual Color getcolor() const;
   virtual double getKd() const;
  
 private:
 float Kd;
  Color color;
  SpecularMaterial(const SpecularMaterial&);
  SpecularMaterial& operator=(const SpecularMaterial&);
  float Ka;
  float Ks;
};

#endif
