#pragma once

#include <glm/glm.hpp>

// A ray
class Ray {
public:
  Ray( const glm::vec3 pos, const glm::vec3 dir ) :
  		pos(pos),
  		dir(dir) {}
  const glm::vec3 pos, dir;
};
