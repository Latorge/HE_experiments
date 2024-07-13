#include "Vertex.hpp"


Vertex::Vertex(int& _id) : position(glm::vec3(0.0, 0.0, 0.0)), id(_id) {
     halfedge  =   nullptr;
}

Vertex::Vertex(float x, float y, float z, int& _id) : position(glm::vec3(x, y, z)), id(_id){
    halfedge  =   nullptr;
}

Vertex::Vertex(glm::vec3 vec, int& _id) : position(vec), id(_id){
    halfedge  =   nullptr;
}

void Vertex::changeComponent(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void Vertex::change(const glm::vec3& vector) {
    position = vector;
}

void Vertex::move(const glm::vec3& vector) {
    position += vector;
}

std::vector<Halfedge*> Vertex::collectFreeHalfedges() {
    std::vector<Halfedge*> freeHalfedges;
    for (auto he : loopCW()) {
        if (he->isFree()) {
            freeHalfedges.push_back(he);
        }
    }
    return freeHalfedges;
}

Halfedge* Vertex::freeHalfedgesInLoop() {
    for (auto he : loopCW()) {
        if (he->isFree()) {
            return he;
        }
    }
    return nullptr;
}

FreeHalfedgeIterator Vertex::freeHalfedgesInLoopIter() {
        return FreeHalfedgeIterator(loopCW().begin(), loopCW().end());
}

std::vector<Halfedge*> Vertex::collectBoundaryHalfedges() {
    std::vector<Halfedge*> boundaryHalfedges;
    for (auto he : loopCW()) {
        if (he->isBoundary()) {
            boundaryHalfedges.push_back(he);
        }
    }
    return boundaryHalfedges;
}

bool Vertex::isFree() {
    if (isIsolated()) {
        return true;
    }
    for (auto he : loopCW()) {
        if (he->isFree()) {
            return true;
        }
    }
    return false;
}

bool Vertex::isIsolated() {
    return halfedge == nullptr;
}

std::vector<Face*> Vertex::commonFacesWithVertex(Vertex* other) {
    std::vector<Face*> faces;
    for (auto he : loopCW()) {
        if (halfedge->face && halfedge->face->hasVertex(other)) {
            faces.push_back(halfedge->face);
        }
    }
    return faces;
}

bool Vertex::matchesPosition(const glm::vec3& pos, float tolerance = 1e-10) {
    glm::vec3 delta = position - pos;
    return glm::length(delta) < tolerance;
}

Halfedge* Vertex::getHalfedgeToVertex(Vertex* other) {
    for (auto he : loopCW()) {
        if (he->twin->vertex == other) {
            return he;
        }
    }
    return nullptr;
}

bool Vertex::isConnectedToVertex(Vertex* other) {
    return getHalfedgeToVertex(other) != nullptr;
}

std::vector<Halfedge*> Vertex::loopCW() {
    std::vector<Halfedge*> edges;
    if (!halfedge) return edges;

    Halfedge* start = halfedge;
    Halfedge* curr = start;
    do {
        edges.push_back(curr);
        curr = curr->twin->next;
    } while (curr != start);

    return edges;
}
/*
glm::vec3 Vertex::calculateVertexNormal() {
    glm::vec3 normal(0.0f);
    std::set<Face*> sharedFaces;
    for (auto he : loopCW()) {
        if (he->face) {
            sharedFaces.insert(he->face);
        }
    }
    for (const auto& face : sharedFaces) {
        glm::vec3 faceNormal = face->normal;
        if (!face->hasVertex(this)) {
            faceNormal = -faceNormal;
        }
        float angleWeight = calculateAngleWeight(face, faceNormal, face->commonVerticesWithVertex(this));
        normal += faceNormal * angleWeight;
    }
    return glm::normalize(normal);
}
*/
float Vertex::calculateAngleWeight(Face* face, const glm::vec3& faceNormal, size_t numCommonVertices) {
    glm::vec3 vertexToFaceCenter = face->calculateCenterPoint() - position;
    return std::acos(glm::dot(faceNormal, vertexToFaceCenter) / (glm::length(faceNormal) * glm::length(vertexToFaceCenter))) / numCommonVertices;
}
