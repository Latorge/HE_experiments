#include "Vertex.hpp"


Vertex::Vertex(int& _id) : position(glm::vec3(0.0, 0.0, 0.0)), id(_id) {
     halfedge  =   nullptr;
}

Vertex::Vertex(float x, float y, float z, int& _id) : position(glm::vec3(x, y, z)), id(_id){
    halfedge  =   nullptr;
}

Vertex::Vertex(glm::vec3 vec, int _id) : position(vec), id(_id){
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
/*
std::vector<Halfedge*> Vertex::allHalfedgesInLoop(Halfedge* start) {
     std::vector<Halfedge*> allHalfedges;
        for (auto it =  beginCW(start); it != endCW(); ++it) {
                Halfedge* he = &(*it);
               if(he->vertex==this)
                allHalfedges.push_back(&(*it));
        }
    return allHalfedges;
}
*/
std::vector<Halfedge*> Vertex::allHalfedgesInLoop(Halfedge* start) {
    std::unordered_set<Halfedge*, HalfedgeHash> uniqueHalfedges;  // Set to store unique halfedges
    std::vector<Halfedge*> allHalfedges;  // Vector to return

    for (auto it = beginCW(start); it != endCW(); ++it) {
        Halfedge* he = &(*it);
        if (he->vertex == this) {  // Ensure that only halfedges whose vertex is this vertex are considered
            uniqueHalfedges.insert(he);  // Insert halfedge into set to ensure uniqueness
        }
    }

    // Transfer unique halfedges from set to vector
    allHalfedges.assign(uniqueHalfedges.begin(), uniqueHalfedges.end());

    return allHalfedges;
}

std::vector<Halfedge*> Vertex::freeHalfedgesInLoop(Halfedge* start) {
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
       // if ((*it).twin && (*it).twin->isFree()) {
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
    std::vector<Face*> commonFaces;

    // Use the CWIterator to iterate over the halfedges in a clockwise direction
    for (auto it = beginCW(); it != endCW(); ++it) {
         // Since the iterator returns a Halfedge reference, get the pointer with the address-of operator
        Halfedge* he = &(*it);
        // Check if the current halfedge has a face and if the face includes the other vertex
        if (he->face && he->face->hasVertex(other)) {
            commonFaces.push_back(he->face);
        }
    }

    return commonFaces;
}

bool Vertex::matchesPosition(const glm::vec3& pos, float tolerance = 1e-10) {
    glm::vec3 delta = position - pos;
    return glm::length(delta) < tolerance;
}

/**
 * Returns the halfedge going from this vertex to the other vertex if any.
 *
 * @param other A pointer to the other vertex to find the halfedge to.
 * @return A pointer to the Halfedge if found, nullptr otherwise.
 */
Halfedge* Vertex::getHalfedgeToVertex(Vertex* other) {
    // Use the CWIterator to iterate over the halfedges in a clockwise direction
    for (auto it = beginCW(); it != endCW(); ++it) {
        // Check if the twin of the current halfedge points to the other vertex
        if ((*it).twin && (*it).twin->vertex == other) {
            return &(*it);
        }
    }
    return nullptr;
}

bool Vertex::isConnectedToVertex(Vertex* other) {
    return getHalfedgeToVertex(other) != nullptr;
}
/*
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
*/
// Function to calculate the normal of the vertex
glm::vec3 Vertex::calculateVertexNormal() {
    glm::vec3 normal(0.0f, 0.0f, 0.0f);
    std::set<Face*> sharedFaces;

    // Collect faces that share this vertex
    for (auto it = beginCW(); it != endCW(); ++it) {
        if ((*it).face) {
            sharedFaces.insert((*it).face);
        }
    }

    // Calculate the weighted sum of face normals
    for (auto face : sharedFaces) {
        auto commonVertices = face->commonVerticesWithVertex(this);
        if (!face->isNormalCalculated())
            face->calculateNormal();
        if (face->normal == glm::vec3(0.0f)) {
            std::cerr << "Cannot calculate normal in vertex module calculateVertexNormal\n";
            return glm::vec3(1.0f, 1.0f, 1.0f);
        }
        glm::vec3 faceNormal = face->normal;

        // Check if the vertex is on the back side of the face
        if (!face->hasVertex(this)) {
            faceNormal = -faceNormal;
        }

        float angleWeight = calculateAngleWeight(face, faceNormal, commonVertices.size());
        normal += faceNormal * angleWeight;
    }

    return glm::normalize(normal);
}

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