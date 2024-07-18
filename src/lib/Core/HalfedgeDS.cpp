#include "HalfedgeDS.hpp"


// Method to clear all the stored components
void HalfedgeDS::clear() {
    // Delete and clear vertices
    for (auto v : vertices) delete v;
    vertices.clear();

    // Delete and clear halfedges
    for (auto he : halfedges) delete he;
    halfedges.clear();

    // Delete and clear faces
    for (auto f : faces) delete f;
    faces.clear();

    // Reset ID counters
    vertexIDCounter = 0;
    halfedgeIDCounter = 0;
    faceIDCounter = 0;
}


// Method to find all unique loops in the half-edge structure
std::vector<std::vector<Halfedge*>> HalfedgeDS::loops() {
    std::vector<std::vector<Halfedge*>> allLoops;
    std::set<Halfedge*> handled;

    for (auto& he : halfedges) {
        if (handled.find(he) == handled.end()) {
            auto loop = he->nextLoop();
            for (auto& loopHe : loop) {
                handled.insert(loopHe);
            }
            allLoops.push_back(loop);
        }
    }

    return allLoops;
}

std::vector<std::vector<Halfedge*>> HalfedgeDS::loopsExp() {
    std::vector<std::vector<Halfedge*>> allLoops;

    for (auto& he : halfedges) {
        if (!he->visited && (!he->isFree() || (he->isFree() && !he->twin->isFree()))) {
            auto loop = he->nextLoop();
            allLoops.push_back(loop);
        }
    }

    // Reset visited flags after processing
    for (auto& he : halfedges) {
        he->visited = false;
    }

    return allLoops;
}

Vertex* HalfedgeDS::addVertexDirect(glm::vec3 position) {
        Vertex* newVertex = new Vertex(position, vertexIDCounter); 
        vertices.push_back(newVertex);
        vertexIDCounter++;
        return newVertex; // Return the ID then increment the counter
    }

Vertex* HalfedgeDS::addVertexFromPosition(glm::vec3 position, bool checkDuplicates, float tolerance)  {
    if (checkDuplicates) {
        for (auto& vertex : getVertices()) {
            if (vertex->matchesPosition(position, tolerance)) {
                return vertex;
            }
        }
    }
    auto v=addVertexDirect(position);
    v->subGroup = -1;
    return v;
}

bool HalfedgeDS::addVertex(Vertex *vertex) {
  vertices.push_back(vertex);
  vertex->id=vertexIDCounter;
  return vertexIDCounter++;  // Return the ID then increment the counter
}

int HalfedgeDS::addFace(Face* face) {
    faces.push_back(face);
    face->id=faceIDCounter;
    return faceIDCounter++;  // Return the ID then increment the counter
}

int HalfedgeDS::addHalfedge(Halfedge* halfedge) {
    halfedges.push_back(halfedge);
    halfedge->id=halfedgeIDCounter;
    return halfedgeIDCounter++;  // Return the ID then increment the counter
}

// Method to remove a halfedge from the vector
bool HalfedgeDS::removeHalfedge(Halfedge* halfedge) {
    auto it = std::find(halfedges.begin(), halfedges.end(), halfedge);
    if (it != halfedges.end()) {
        halfedges.erase(it);
        delete halfedge; // Deallocate memory if halfedges are dynamically allocated
        //halfedgeIDCounter--;
        return true;
    }
    return false;
}

// Method to remove a halfedge from the vector
bool HalfedgeDS::containsHalfedge(Halfedge* halfedge) {
    auto it = std::find(halfedges.begin(), halfedges.end(), halfedge);
    if (it != halfedges.end()) {
        return true;
    }
    return false;
}



bool HalfedgeDS::removeFace(Face* face) {
        auto it = std::find(faces.begin(), faces.end(), face);
        if (it != faces.end()) {
            faces.erase(it);
            delete face;  // Free the memory if Faces are dynamically allocated
            //faceIDCounter--;
            return true;
        }
        return false;  // Face not found
}

bool HalfedgeDS::removeVertex(Vertex* vertex) {
    auto it = std::find(vertices.begin(), vertices.end(), vertex);
    if (it != vertices.end()) {
        // Before removing the vertex, you might need to handle associated halfedges
        // For example, you might need to update or remove halfedges that reference this vertex
        //updateHalfedgesBeforeVertexRemoval(vertex);
        // Now remove the vertex from the list
        vertices.erase(it);
        // Optional: if the Vertex objects are dynamically allocated
        delete vertex;
        return true; // Vertex successfully removed
    }
    return false; // Vertex not found
}
    

int HalfedgeDS::getNextFaceID() const {
    return faceIDCounter;
}

 // Destructor to properly clean up memory
HalfedgeDS::~HalfedgeDS() {
    clear();
}