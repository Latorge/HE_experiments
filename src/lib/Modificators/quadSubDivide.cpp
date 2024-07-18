#include "Modificators.hpp"

namespace Modificators {
   

    void quadSubDivideStruct(HalfedgeDS& structDS) {
        quadSubDivide(structDS);  // Assume this function is implemented elsewhere

        std::vector<Face*> selectedFaces;

        // Collect faces with 8 vertices
        for (auto& face : structDS.getFaces()) {
            if (face->calculateNumberVertex() == 8) {
                selectedFaces.push_back(face);
            }
        }

        float ratio = 0.5f;
        std::vector<Halfedge*> newHalfEdges;

        // Now process the collected faces
        for (auto& face : selectedFaces) {
            std::vector<Vertex*> newVertices;
            Halfedge* currentHalfedge = face->halfedge;
            int vertexIndex = 0;
            do {
                if (!currentHalfedge->visited && (vertexIndex == 1 || vertexIndex == 5)) {
                    newVertices.push_back(currentHalfedge->vertex);
                    currentHalfedge->visited = true;
                }
                vertexIndex++;
                currentHalfedge = currentHalfedge->next;
            } while (currentHalfedge != face->halfedge);

            if (newVertices.size() >= 2) {
                Halfedge* newHalfEdge = Operations::cutFace(structDS, face, newVertices[0], newVertices[1]);
                newHalfEdges.push_back(newHalfEdge);
            }
        }

        // Split edges at calculated mid-points
        for (auto halfEdge : newHalfEdges) {
            Operations::splitEdgeRatio(structDS, halfEdge, ratio);
        }


        //struct.loops();  // Assuming this function does some internal restructuring or cleanup

        selectedFaces.clear();
        for (auto& face : structDS.getFaces()) {
            if (face->calculateNumberVertex() == 6) {
                selectedFaces.push_back(face);
            }
        }

        newHalfEdges.clear();

        // Process the collected faces
        for (auto& face : selectedFaces) {
            std::vector<Vertex*> newVertices;
            Halfedge* currentHalfedge = face->halfedge;
            int vertexIndex = 0;
            do {
                if (!currentHalfedge->visited && (vertexIndex == 1 || vertexIndex == 4)) {
                    newVertices.push_back(currentHalfedge->vertex);
                    currentHalfedge->visited = true;
                }
                vertexIndex++;
                currentHalfedge = currentHalfedge->next;
            } while (currentHalfedge != face->halfedge);

            if (newVertices.size() >= 2) {  // Ensure we have at least two vertices to operate on
                Halfedge* newHalfEdge = Operations::cutFace(structDS, face, newVertices[0], newVertices[1]);
                newHalfEdges.push_back(newHalfEdge);
            }
        }


    }

   void quadSubDivide(HalfedgeDS& structDS) {
        // First, mark all halfedges as not visited
        for (auto halfedge : structDS.getHalfedges()) {
            halfedge->visited = false;
        }

        // Collect halfedges that need to be split
        std::vector<Halfedge*> toSplit;
        float ratio = 0.5f;  // The split ratio

        for (auto halfedge : structDS.getHalfedges()) {
            if (!halfedge->visited && !halfedge->twin->visited) {
                toSplit.push_back(halfedge);  // Collect halfedge for later processing
                halfedge->visited = true;       // Mark it visited to avoid re-processing
            }
        }

        // Split each collected halfedge in a separate loop
        for (auto halfedge : toSplit) {
            Operations::splitEdgeRatio(structDS, halfedge, ratio);
        }

        // Optionally, reset visited flags if they will be used later for other purposes
        for (auto halfedge : structDS.getHalfedges()) {
            halfedge->visited = false;
        }
    }



}