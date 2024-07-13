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

std::vector<Halfedge*> Vertex::freeHalfedgesInLoop(Halfedge* start = nullptr) {
        std::vector<Halfedge*> freeHalfedges;
        for (auto it =  beginCW(start); it != endCW(); ++it) {
            if ((*it).twin && (*it).twin->isFree()) {
                freeHalfedges.push_back((*it).twin);
            }
        }
    return freeHalfedges;
}

Halfedge* Vertex::freeHalfedgesInLoopNext(Halfedge* start) {
    if (!start) start = this->halfedge; // Use the vertex's halfedge if no start is provided

    int count = 0;  // Counter to find the second free halfedge

    // Iterate over the halfedges in a clockwise direction
    for (auto it =  beginCW(start); it != endCW(); ++it) {
        // Check if the twin of the current halfedge exists and is free
        if ((*it).twin && (*it).twin->isFree()) {
            count++;
            if (count == 2) {
                return (*it).twin; // Return the second free halfedge if found
            }
        }
    }

    return nullptr; // Return nullptr if less than two free halfedges exist
}


Halfedge* Vertex::findBoundaryHalfedge(Halfedge* halfIn, Halfedge* halfOut) {
    Halfedge* g = nullptr;

    // Start iteration from the halfedge outwards, assuming halfOut->vertex points to the starting vertex
    auto it = halfOut->vertex->beginCW(halfOut);
    while (it.hasNext()) {
        Halfedge* he = it.next();
        
        // Check if this halfedge is free and is not the input halfedge
        if (he->isFree() && he != halfIn) {
            g = he;
            break;
        }

        // Make sure to not infinitely loop; stop if we come back to the starting point
        if (he == halfOut) break;
    }

    return g;
}

// Check if the vertex is free (any of its halfedges has no face)
bool Vertex::isFree() {
    if (isIsolated()) {
        return true;
    }
    for (CWIterator it = beginCW(); it != endCW(); ++it) {
        if ((*it).isFree()) {
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
        if (halfedge->twin && halfedge->twin->vertex == other) {
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

 // Prefix increment
Vertex::CWIterator& Vertex::CWIterator::operator++() {
    if (current && current->twin) {
        current = current->twin->next;
        if (current == start) {
            if (!firstPass) {
                current = nullptr;  // End the iteration
            }
            firstPass = false;
        }
    }
    return *this;
}

// Postfix increment
Vertex::CWIterator Vertex::CWIterator::operator++(int) {
    CWIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool Vertex::CWIterator::hasNext() const {
    return current != nullptr && (firstPass || current != start);
}

Halfedge* Vertex::CWIterator::next() {
    if (!hasNext()) return nullptr;
    Halfedge* result = current;
    operator++();  // Move the iterator forward
    return result;
}

void Vertex::CWIterator::reset() {
    current = start;
    firstPass = true;
}


 // Prefix decrement
Vertex::CCWIterator& Vertex::CCWIterator::operator--() {
    if (current && current->prev && current->prev->twin) {
        current = current->prev->twin;
        if (current == start) {
            if (!firstPass) {
                current = nullptr; // End the iteration
            }
            firstPass = false;
        }
    }
    return *this;
}

// Postfix decrement
Vertex::CCWIterator Vertex::CCWIterator::operator--(int) {
    CCWIterator tmp = *this;
    --(*this);
    return tmp;
}