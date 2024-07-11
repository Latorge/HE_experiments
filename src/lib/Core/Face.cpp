#include "Face.hpp"

Face::Face(Halfedge*  _he, int _id)  : halfedge(_he), id(_id), normalFlag(false) {}

glm::vec3 Face::getNormal() {
    if (!normalFlag) {

        auto prevPos = halfedge->prev->vertex->position;
        auto currentPos = halfedge->vertex->position;
        auto nextPos = halfedge->next->vertex->position;
        normal = glm::normalize(glm::cross(nextPos - currentPos, prevPos - currentPos));
        normalFlag = true;
    }
    return normal;
}

glm::vec3 Face::calculateCenterPoint() {
    glm::vec3 sum(0.0f);
    int count = 0;
    auto start = halfedge;
    auto current = start;

    do {
        sum += current->vertex->position;
        count++;
        //current = current->next;
    } while (current != start);

    centerPoint = sum / static_cast<float>(count);
    return centerPoint;
}

bool Face::isFront(const glm::vec3& position) {
    glm::vec3 viewVector = glm::normalize(position - halfedge->vertex->position);
    return glm::dot(viewVector, getNormal()) >= 0;
}

Halfedge* Face::halfedgeFromPosition(const glm::vec3& position, float tolerance = 1e-10) {
    auto current = halfedge;
    do {
        if (current->containsPoint(position, tolerance)) {
            return current;
        }
        //current = current->next;
    } while (current != halfedge);
    return nullptr;
}

Vertex*  Face::vertexFromPosition(const glm::vec3& position, float tolerance = 1e-10) {
    auto current = halfedge;
    do {
        glm::vec3 result = glm::abs(current->vertex->position - position);
        if (glm::length(result) < tolerance) {
            return current->vertex;
        }
       // current = current->next;
    } while (current != halfedge);
    return nullptr;
}

bool Face::hasVertex(Vertex* vertex) {
    for (auto& he : *halfedge) {
        if (he.vertex == vertex) {
            return true;
        }
    }
    return false;
}
