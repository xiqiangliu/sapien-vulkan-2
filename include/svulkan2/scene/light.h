#pragma once
#include "node.h"
#include "svulkan2/common/config.h"
#include <vector>

namespace svulkan2 {
namespace scene {

class DirectionalLight : public Node {
  glm::vec3 mColor{0, 0, 0};
  float mSoftness{0}; // ray tracing only
  bool mCastShadow{};
  float mShadowNear{0};
  float mShadowFar{10};
  float mShadowScaling{10};
  uint32_t mShadowMapSize{2048};

public:
  DirectionalLight(std::string const &name = "");
  inline void setColor(glm::vec3 const &color) { mColor = color; }
  inline glm::vec3 getColor() const { return mColor; }
  inline void setSoftness(float softness) { mSoftness = softness; }
  inline float getSoftness() const { return mSoftness; }
  void enableShadow(bool enable);
  inline bool isShadowEnabled() const { return mCastShadow; }
  void setShadowParameters(float near, float far, float scaling, uint32_t size);
  glm::vec3 getDirection() const;
  void setDirection(glm::vec3 const &dir);

  inline float getShadowNear() const { return mShadowNear; }
  inline float getShadowFar() const { return mShadowFar; }
  inline float getShadowScaling() const { return mShadowScaling; }
  inline uint32_t getShadowMapSize() const { return mShadowMapSize; }

  glm::mat4 getShadowProjectionMatrix() const;
};

class PointLight : public Node {
  glm::vec3 mColor{0, 0, 0};
  bool mCastShadow{};
  float mShadowNear{0};
  float mShadowFar{10};
  uint32_t mShadowMapSize{2048};

public:
  static std::array<glm::mat4, 6> getModelMatrices(glm::vec3 const &center);

  PointLight(std::string const &name = "");
  inline void setColor(glm::vec3 const &color) { mColor = color; }
  inline glm::vec3 getColor() const { return mColor; }
  void enableShadow(bool enable);
  inline bool isShadowEnabled() const { return mCastShadow; }
  void setShadowParameters(float near, float far, uint32_t size);
  glm::mat4 getShadowProjectionMatrix() const;

  inline float getShadowNear() const { return mShadowNear; }
  inline float getShadowFar() const { return mShadowFar; }
  inline uint32_t getShadowMapSize() const { return mShadowMapSize; }
};

class SpotLight : public Node {
  glm::vec3 mColor{0, 0, 0};
  bool mCastShadow{};
  float mShadowNear{0};
  float mShadowFar{10};
  float mFovSmall = 1.5708;
  float mFov = 1.5708;
  uint32_t mShadowMapSize{2048};

public:
  SpotLight(std::string const &name = "");
  inline void setColor(glm::vec3 const &color) { mColor = color; }
  inline glm::vec3 getColor() const { return mColor; }
  void enableShadow(bool enable);
  inline bool isShadowEnabled() const { return mCastShadow; }
  void setShadowParameters(float near, float far, uint32_t size);
  void setDirection(glm::vec3 const &dir);
  glm::vec3 getDirection() const;
  void setFov(float fov);
  float getFov() const;

  void setFovSmall(float fov);
  float getFovSmall() const;

  inline float getShadowNear() const { return mShadowNear; }
  inline float getShadowFar() const { return mShadowFar; }
  inline uint32_t getShadowMapSize() const { return mShadowMapSize; }

  glm::mat4 getShadowProjectionMatrix() const;
};

class TexturedLight : public SpotLight {
  std::shared_ptr<resource::SVTexture> mTexture{};

public:
  using SpotLight::SpotLight;
  void setTexture(std::shared_ptr<resource::SVTexture> texture);
  inline std::shared_ptr<resource::SVTexture> getTexture() const {
    return mTexture;
  };
};

class ParallelogramLight : public Node {
public:
  ParallelogramLight(std::string const &name = "");
  inline void setColor(glm::vec3 const &color) { mColor = color; }
  inline glm::vec3 getColor() const { return mColor; }
  inline void setShape(glm::vec3 const &edge0, glm::vec3 const &edge1) {
    mEdge0 = edge0;
    mEdge1 = edge1;
  }
  inline glm::vec3 getEdge0() const { return mEdge0; }
  inline glm::vec3 getEdge1() const { return mEdge1; }

private:
  glm::vec3 mColor{0, 0, 0};
  glm::vec3 mEdge0{0, 1, 0};
  glm::vec3 mEdge1{0, 0, 1};
};

} // namespace scene
} // namespace svulkan2
