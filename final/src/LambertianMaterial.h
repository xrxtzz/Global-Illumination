
#ifndef LambertianMaterial_h
#define LambertianMaterial_h

#include "Material.h"
#include "Color.h"

class LambertianMaterial : public Material {
 public:
  LambertianMaterial(const Color& color, float Kd, float Ka);
  virtual ~LambertianMaterial();

  virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const;
 virtual Color getcolor() const;
 virtual double getKd() const;
    
 private:
	float Kd;
	Color color;
  LambertianMaterial(const LambertianMaterial&);
  LambertianMaterial& operator=(const LambertianMaterial&);

  

  float Ka;
};

#endif
