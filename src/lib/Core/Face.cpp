#include "Face.hpp"

Face::Face(Halfedge*  _he, int _id)  : halfedge(_he), id(_id), normalFlag(false) {}

glm::vec3 Face::getNormal(bool useNormalFlag) {
    //if (!normalFlag && useNormalFlag)
    {
        auto prevPos = halfedge->prev->vertex->position;
        auto currentPos = halfedge->vertex->position;
        auto nextPos = halfedge->next->vertex->position;
        normal = glm::normalize(glm::cross(nextPos - currentPos, prevPos - currentPos));
        normalFlag = true;
    }
    return normal;
}

void Face::calculateNormal()
{
    auto prevPos = halfedge->prev->vertex->position;
    auto currentPos = halfedge->vertex->position;
    auto nextPos = halfedge->next->vertex->position;
    normal = glm::normalize(glm::cross(nextPos - currentPos, prevPos - currentPos));
    normalFlag = true;
}

glm::vec3 Face::calculateCenterPoint() {
    glm::vec3 sum(0.0f);
    int count = 0;
    auto start = halfedge;
    auto current = start;
/*
    do {
        sum += current->vertex->position;
        count++;
        //current = current->next;
    } while (current != start);
*/

    auto vertices=this->getVertices();
    for(const auto vertex:vertices)
    {
        sum += vertex->position;
        count++;
    }
    if(count==0)
    {
        return glm::vec3(0.0f);
    }
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

// Returns the face halfedge starting from the given vertex
Halfedge* Face::halfedgeFromVertex(Vertex* vertex) {
    if (!this->halfedge) return nullptr; // No halfedges to search from

    Halfedge* start = this->halfedge;
    Halfedge* current = start;

    do {
        if (current->vertex == vertex) {
            return current;
        }
        current = current->next;
    } while (current != start);

    return nullptr;  // No halfedge starts from the given vertex
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


std::vector<Vertex*> Face::getVertices() {
    std::vector<Vertex*> vertices;
    if (!this->halfedge) return vertices;  // return empty if no halfedges

    Halfedge* start = this->halfedge;
    Halfedge* currentHalfedge = start;

    do {
        vertices.push_back(currentHalfedge->vertex);
        currentHalfedge = currentHalfedge->next;
    } while (currentHalfedge != start);

    return vertices;
}

int Face::calculateNumberVertex()  // possible to add some cache
{
    std::vector<Vertex*> vertices;
    if (!this->halfedge) return 0;  // return empty if no halfedges

    Halfedge* start = this->halfedge;
    Halfedge* currentHalfedge = start;

    do {
        vertices.push_back(currentHalfedge->vertex);
        currentHalfedge = currentHalfedge->next;
    } while (currentHalfedge != start);

    return vertices.size();
}

std::vector<Halfedge*> Face::getHalfedges() {
    std::vector<Halfedge*> halfedges;
    if (!this->halfedge) return halfedges;  // return empty if no halfedges

    Halfedge* start = this->halfedge;
    Halfedge* currentHalfedge = start;

    do {
        halfedges.push_back(currentHalfedge);
        currentHalfedge = currentHalfedge->next;
    } while (currentHalfedge != start);

    return halfedges;
}

std::vector<Vertex*> Face::commonVerticesWithVertex(Vertex* vertex) {
    std::vector<Vertex*> commonVertices;
    std::vector<Vertex*> vertices = this->getVertices();

    for (Vertex* vertexOfFace : vertices) {
        if (vertexOfFace != vertex) {
            commonVertices.push_back(vertexOfFace);
        }
    }

    return commonVertices;
}

bool Face::isNormalCalculated() const {
    return normalFlag;
}



