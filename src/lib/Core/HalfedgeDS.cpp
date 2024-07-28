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

Vertex* HalfedgeDS::addVertex(glm::vec3 position) {
  Vertex *vertex =new Vertex(position);
  vertices.push_back(vertex);
  vertex->id=vertexIDCounter;
  return vertex;  // Return the ID then increment the counter
}

int HalfedgeDS::addHalfedge(Halfedge* halfedge) {
    halfedges.push_back(halfedge);
    halfedge->id=halfedgeIDCounter;
    return halfedgeIDCounter++;  // Return the ID then increment the counter
}

Halfedge* HalfedgeDS::addHalfedge(Vertex* vertex) {
    Halfedge* halfEdge=new Halfedge(vertex);
    halfedges.push_back(halfEdge);
    halfEdge->id=halfedgeIDCounter;
    return halfEdge;
}

int HalfedgeDS::addFace(Face* face) {
    faces.push_back(face);
    face->id=faceIDCounter;
    return faceIDCounter++;  // Return the ID then increment the counter
}

Face* HalfedgeDS::addFace(Halfedge* halfedge) {
    Face* face=new Face(halfedge);
    faces.push_back(face);
    face->id=faceIDCounter;
    return face;  // Return the ID then increment the counter
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

void HalfedgeDS::removeFreeVertices() {
        std::set<Vertex*> verticesWithHalfedge;

        // Collect all vertices linked to halfedges
        for (auto& he : halfedges) {
            verticesWithHalfedge.insert(he->vertex);
        }

        // Collect vertices that are not linked to any halfedge
        std::vector<Vertex*> verticesToDelete;
        for (auto& v : vertices) {
            if (verticesWithHalfedge.find(v) == verticesWithHalfedge.end()) {
                verticesToDelete.push_back(v);
            }
        }

        // Remove the collected vertices
        auto newEnd = std::remove_if(vertices.begin(), vertices.end(),
            [&verticesToDelete](Vertex* v) {
                return std::find(verticesToDelete.begin(), verticesToDelete.end(), v) != verticesToDelete.end();
            });
        vertices.erase(newEnd, vertices.end());

        // Assuming memory management responsibility, delete the vertices
        for (auto& v : verticesToDelete) {
            delete v;
        }
    }

void HalfedgeDS::copyFrom(const HalfedgeDS &other)
{
    std::map<Vertex*, Vertex*> vertexMap;
    std::map<Halfedge*, Halfedge*> halfedgeMap;
    std::map<Face*, Face*> faceMap;

    // Copy vertices
    for (auto v : other.getVertices()) {
        Vertex* newVertex = new Vertex(*v);  // Assuming Vertex has a copy constructor
        vertices.push_back(newVertex);
        vertexMap[v] = newVertex;
    }

    // Copy halfedges
    for (auto he : other.getHalfedges()) {
        Halfedge* newHalfedge = new Halfedge(*he);  // Assuming Halfedge has a copy constructor
        halfedges.push_back(newHalfedge);
        halfedgeMap[he] = newHalfedge;
    }

    // Copy faces
    for (auto f : other.getFaces()) {
        Face* newFace = new Face(*f);  // Assuming Face has a copy constructor
        faces.push_back(newFace);
        faceMap[f] = newFace;
    }

    // Re-link all halfedge pointers
    for (auto he : halfedges) {
        he->vertex = vertexMap[he->vertex];
        he->next = halfedgeMap[he->next];
        he->prev = halfedgeMap[he->prev];
        he->twin = halfedgeMap[he->twin];
        he->face = faceMap[he->face];
    }

    // Update vertex halfedges and face halfedges
    for (auto v : vertices) {
        v->halfedge = halfedgeMap[v->halfedge];
    }

    for (auto f : faces) {
        f->halfedge = halfedgeMap[f->halfedge];
    }
}

void HalfedgeDS::mergeFrom( HalfedgeDS& other) {
    /*
    // Directly append vertices
    this->vertices.insert(this->vertices.end(), other.vertices.begin(), other.vertices.end());

    // Directly append halfedges
    this->halfedges.insert(this->halfedges.end(), other.halfedges.begin(), other.halfedges.end());

    // Directly append faces
    this->faces.insert(this->faces.end(), other.faces.begin(), other.faces.end());
    */

   std::map<Vertex*, Vertex*> vertexMap;
    std::map<Halfedge*, Halfedge*> halfedgeMap;
    std::map<Face*, Face*> faceMap;

    // Add vertices from 'other' to 'this'
    for (auto v : other.getVertices()) {
        Vertex* newVertex = new Vertex(*v);  // Copy constructor
        this->vertices.push_back(newVertex);
        vertexMap[v] = newVertex;
    }

    // Add halfedges from 'other' to 'this'
    for (auto he : other.getHalfedges()) {
        Halfedge* newHalfedge = new Halfedge(*he);  // Copy constructor
        this->halfedges.push_back(newHalfedge);
        halfedgeMap[he] = newHalfedge;
    }

    // Add faces from 'other' to 'this'
    for (auto f : other.getFaces()) {
        Face* newFace = new Face(*f);  // Copy constructor
        this->faces.push_back(newFace);
        faceMap[f] = newFace;
    }

    // Re-link all halfedge pointers in the newly added elements
    for (auto he : this->halfedges) {
        if (halfedgeMap.count(he->vertex->halfedge)) {  // Ensure the halfedge pointer of the vertex is within the added elements
            he->vertex = vertexMap[he->vertex];
        }
        if (halfedgeMap.count(he->next)) {
            he->next = halfedgeMap[he->next];
        }
        if (halfedgeMap.count(he->prev)) {
            he->prev = halfedgeMap[he->prev];
        }
        if (halfedgeMap.count(he->twin)) {
            he->twin = halfedgeMap[he->twin];
        }
        if (faceMap.count(he->face)) {
            he->face = faceMap[he->face];
        }
    }

    // Update vertex and face halfedges if they are pointing to halfedges from the added elements
    for (auto v : this->vertices) {
        if (halfedgeMap.count(v->halfedge)) {
            v->halfedge = halfedgeMap[v->halfedge];
        }
    }

    for (auto f : this->faces) {
        if (halfedgeMap.count(f->halfedge)) {
            f->halfedge = halfedgeMap[f->halfedge];
        }
    }
}

