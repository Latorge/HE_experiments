#include "Operations.hpp"

namespace Operations {
    


    void processFaceRecursive(HalfedgeDS& halfedgeDS, Face* face, int numIterations) {
        if (numIterations == 0 || !face) {
            return;
        }

        // Get the halfedges of the face
        std::vector<Halfedge*> halfedges = face->getHalfedges();
        if (halfedges.size() < 4) {  // Ensure there are enough edges
            return;
        }

        // Find the two longest edges
        std::nth_element(halfedges.begin(), halfedges.begin() + 2, halfedges.end(),
                        [](Halfedge* a, Halfedge* b) {
                            return glm::length(a->vertex->position - a->twin->vertex->position) > 
                                    glm::length(b->vertex->position - b->twin->vertex->position);
                        });

        // Split the two longest edges at the midpoint
        Vertex* newVertex1 = Operations::splitEdgeRatio(halfedgeDS, halfedges[0], 0.5);
        Vertex* newVertex2 = Operations::splitEdgeRatio(halfedgeDS, halfedges[1], 0.5);

        // Perform the cut face operation
        Operations::cutFace(halfedgeDS, face, newVertex1, newVertex2, true);

        // Recursively process the next iteration
        processFaceRecursive(halfedgeDS, face, numIterations - 1);  // Assuming face is updated or determine the new face
    }

// Usage example
//processFaceRecursive(halfedgeDS01, halfedgeDS01.getFace(0), 5);

    void processFaceRecursiveOppositeEdges(HalfedgeDS& halfedgeDS, Face* face, int numIterations) {
        if (numIterations == 0 || !face) {
            return;
        }

        // Get the halfedges of the face
        std::vector<Halfedge*> halfedges = face->getHalfedges();
        if (halfedges.size() < 4) {  // Ensure there are enough edges for meaningful 'opposite' calculation
            return;
        }

        // Find the longest edge
        auto longestIt = std::max_element(halfedges.begin(), halfedges.end(), 
                                        [](Halfedge* a, Halfedge* b) {
                                            return glm::length(a->vertex->position - a->twin->vertex->position) <
                                                    glm::length(b->vertex->position - b->twin->vertex->position);
                                        });

        // Calculate the index of the longest edge and find the opposite edge
        size_t longestIndex = std::distance(halfedges.begin(), longestIt);
        size_t oppositeIndex = (longestIndex + halfedges.size() / 2) % halfedges.size();  // Assuming even number of edges

        Halfedge* longestEdge = *longestIt;
        Halfedge* oppositeEdge = halfedges[oppositeIndex];

        // Split the longest and opposite edges at the midpoint
        Vertex* newVertex1 = Operations::splitEdgeRatio(halfedgeDS, longestEdge, 0.5);
        Vertex* newVertex2 = Operations::splitEdgeRatio(halfedgeDS, oppositeEdge, 0.5);

        // Perform the cut face operation
        Operations::cutFace(halfedgeDS, face, newVertex1, newVertex2, true);

        // Recursively process the next iteration
        processFaceRecursiveOppositeEdges(halfedgeDS, face, numIterations - 1);  // Assuming face is updated or determine the new face
    }


}
