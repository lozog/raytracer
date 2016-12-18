#pragma once

#include <glm/glm.hpp>
#include "Material.hpp"

struct IntersectInfo {
  IntersectInfo();
  Material* material;
  glm::vec3 point;
  glm::vec3 normal;
};
