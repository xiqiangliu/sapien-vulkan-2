#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace svulkan2 {
namespace math {

inline float fov2focal(float fov, float length) {
  return length / 2.f / glm::tan(fov / 2.f);
}

inline float focal2fov(float f, float length) {
  return 2.f * glm::atan(length / 2.f / f);
}

inline glm::mat4 ortho(float left, float right, float bottom, float top,
                       float near, float far) {
  float tx = -(right + left) / (right - left);
  float ty = -(bottom + top) / (bottom - top);
  float tz = -near / (far - near);

  return glm::mat4(2 / (right - left), 0, 0, 0, 0, 2 / (bottom - top), 0, 0, 0,
                   0, -1 / (far - near), 0, tx, ty, tz, 1);
}

inline glm::mat4 perspective(float fovy, float aspect, float near, float far) {
  float f = 1 / glm::tan(fovy / 2);
  return glm::mat4(f / aspect, 0, 0, 0, 0, -f, 0, 0, 0, 0, -far / (far - near),
                   -1, 0, 0, -far * near / (far - near), 0);
}

inline glm::mat4 fullPerspective(float near, float far, float fx, float fy,
                                 float cx, float cy, float width, float height,
                                 float skew) {
  glm::mat4 mat(1);
  mat[0][0] = (2.f * fx) / width;
  mat[1][1] = -(2.f * fy) / height;
  mat[2][2] = -far / (far - near);
  mat[3][2] = -far * near / (far - near);
  mat[2][3] = -1.f;
  mat[2][0] = -2.f * cx / width + 1;
  mat[2][1] = -2.f * cy / height + 1;
  mat[3][3] = 0.f;
  mat[1][0] = -2 * skew / width;
  return mat;
}

} // namespace math
} // namespace svulkan2
