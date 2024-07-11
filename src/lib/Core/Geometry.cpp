#include "Geometry.hpp"

namespace Geometry {

int orient3D(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d) {
    glm::mat4 matrix = {
        {a.x, a.y, a.z, 1.0},
        {b.x, b.y, b.z, 1.0},
        {c.x, c.y, c.z, 1.0},
        {d.x, d.y, d.z, 1.0}
    };
    double det = glm::determinant(matrix);

    if (det > EPSILON) {
        return 1;
    } else if (det < -EPSILON) {
        return -1;
    }
    return 0;
}

int frontSide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d) {
    return orient3D(d, b, c, a);
}

bool sameSide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const glm::vec3& e) {
    return (orient3D(a, b, c, d) > 0) == (orient3D(a, b, c, e) > 0);
}

}
