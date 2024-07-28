#include "Modificators.hpp"

namespace Modificators {
  
    void triangleSubDivideStruct(HalfedgeDS& structDS) {

        std::vector<Face*> toProcess;
        // Collect faces based on vertex count
        for (auto& face : structDS.getFaces()) {
            int vertexCount = face->calculateNumberVertex();
            if (vertexCount == 4) {
                toProcess.push_back(face);
            } else if (vertexCount > 4) {
                toProcess.push_back(face);
            }
        }

        for (auto& face : toProcess) {
            int vertexCount = face->calculateNumberVertex();
            if (vertexCount == 4) {

                // For quad faces, find opposite vertices and cut the face
                Vertex* startVertex = face->halfedge->vertex;
                Vertex* endVertex = face->halfedge->next->next->vertex;
                Halfedge* newHalfEdge = Operations::cutFace(structDS, face, startVertex, endVertex);

            } else if (vertexCount > 4) {
                glm::vec3 centroid = face->calculateCenterPoint();
                Vertex* centerVertex = new Vertex(centroid);
                structDS.addVertex(centerVertex); // Assume this method exists to add a vertex to the structure

                std::vector<Halfedge*> newHalfEdges;
                std::vector<Face*> newFaces;
                Halfedge* startHalfedge = face->halfedge;
                Halfedge* currentHalfedge = startHalfedge;

                // Temporarily store the original halfedges for linking new halfedges later
                std::vector<Halfedge*> originalHalfEdges=face->getHalfedges();

                std::vector<Halfedge*> listHalfEdgesFromCenter;
                std::vector<Halfedge*> listHalfEdgesToCenter;

                // Now create new halfedges and faces
                for (size_t i = 0; i < originalHalfEdges.size(); ++i) {

                    Vertex* currentVertex = originalHalfEdges[i]->vertex;
                    Halfedge* nextOriginalHalfEdge = originalHalfEdges[(i + 1) % originalHalfEdges.size()];

                    // Create new halfedges
                    Halfedge* halfEdgeToCenter = structDS.addHalfedge(currentVertex);
                    Halfedge* halfEdgeFromCenter = structDS.addHalfedge(centerVertex);

                    // Link the new halfedges to each other and the center vertex
                    halfEdgeToCenter->twin = halfEdgeFromCenter;
                    halfEdgeFromCenter->twin = halfEdgeToCenter;

                    listHalfEdgesFromCenter.push_back(halfEdgeFromCenter);
                    listHalfEdgesToCenter.push_back(halfEdgeToCenter);
                }

                // Final construction of faces and linking of halfedges
                for (size_t i = 0; i < originalHalfEdges.size(); ++i) {
                    // Current and next indices
                    size_t nextIndex = (i + 1) % originalHalfEdges.size();

                    // Current and next halfedges to and from the center
                    Halfedge* halfEdgeFromCenter = listHalfEdgesFromCenter[i];
                    Halfedge* halfEdgeToCenter = listHalfEdgesToCenter[nextIndex];
                    //Halfedge* nextHalfEdgeFromCenter = listHalfEdgesFromCenter[nextIndex];

                    // Original halfedge between the two vertices
                    Halfedge* currentOriginalHalfEdge = originalHalfEdges[i];

                    // Setup the next and prev links for a triangular face
                    halfEdgeFromCenter->next = currentOriginalHalfEdge;
                    halfEdgeFromCenter->prev = halfEdgeToCenter;

                    currentOriginalHalfEdge->next = halfEdgeToCenter;
                    currentOriginalHalfEdge->prev = halfEdgeFromCenter;
                   
                    halfEdgeToCenter->next = halfEdgeFromCenter;
                    halfEdgeToCenter->prev = currentOriginalHalfEdge;

                    // Create new face and assign it to the halfedges
                    Face* newFace = new Face(halfEdgeToCenter);
                    structDS.addFace(newFace);  // Assume this method exists to add a face to the structure
                    newFaces.push_back(newFace);

                    // Assign the new face to all halfedges
                    halfEdgeToCenter->face = newFace;
                    halfEdgeFromCenter->face = newFace;
                    currentOriginalHalfEdge->face = newFace;
                }


                // Optionally, you can now remove the original face
                structDS.removeFace(face);
            }
        }
    }



}