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

Vertex* HalfedgeDS::addVertex(glm::vec3 position, bool checkDuplicates, float tolerance)  {
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