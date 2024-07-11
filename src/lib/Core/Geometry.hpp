#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Geometry {
const double EPSILON = 1e-10;

int orient3D(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d);
int frontSide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d);
bool sameSide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const glm::vec3& e);
}

#endif // GEOMETRY_HPP
