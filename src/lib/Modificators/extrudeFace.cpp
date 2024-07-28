#include "Modificators.hpp"

namespace Modificators {
   
    Face* extrudeFace(HalfedgeDS& structDS, Face* face, float distExtrude, float amountExtrude) {
        glm::vec3 faceNormal = face->getNormal(false);
        glm::vec3 faceCenter = face->calculateCenterPoint();

        auto faceVertexList = face->getVertices();
        auto faceHalfEdgeList = face->getHalfedges();

        glm::vec3 newCenterPoint = faceCenter + faceNormal * static_cast<float>(distExtrude);

        std::vector<glm::vec3> directionList;
        std::vector<Vertex*> newVertices;

        for (auto vertex : faceVertexList) {
            glm::vec3 displacement = vertex->position - faceCenter;
            directionList.push_back(displacement);
        }

        for (auto& displacement : directionList) {
            float displacementLength = glm::length(displacement);
            glm::vec3 displacementNormal = glm::normalize(displacement);
            float extrudeAmount = amountExtrude * displacementLength;
            glm::vec3 newPosition = newCenterPoint + displacementNormal * static_cast<float>(extrudeAmount);
            Vertex* newVertex = new Vertex(newPosition);
            structDS.addVertex(newVertex);
            newVertices.push_back(newVertex);
        }

        std::vector<Halfedge*> upperTwins, leftHalfEdges, rightHalfEdges;
        std::vector<Face*> newFaces;

        for (size_t i = 0; i < newVertices.size(); ++i) {
            size_t nextIndex = (i + 1) % newVertices.size();

            Vertex* vs = faceVertexList[i];
            Vertex* vf = faceVertexList[nextIndex];
            Vertex* vs_new = newVertices[i];
            Vertex* vf_new = newVertices[nextIndex];

            Halfedge* bottomHalfEdge = faceHalfEdgeList[i];

            Face* newFace = new Face(bottomHalfEdge);
            structDS.addFace(newFace);
            newFaces.push_back(newFace);

            Halfedge* rightHalfEdge = structDS.addHalfedge(vf);
            Halfedge* upperHalfEdge = structDS.addHalfedge(vf_new);
            Halfedge* upperHalfEdgeTwin = structDS.addHalfedge(vs_new);
            upperTwins.push_back(upperHalfEdgeTwin);
            Halfedge* leftHalfEdge = structDS.addHalfedge(vs_new);
            leftHalfEdges.push_back(leftHalfEdge);
            rightHalfEdges.push_back(rightHalfEdge);

            bottomHalfEdge->face = newFace;
            bottomHalfEdge->next = rightHalfEdge;
            bottomHalfEdge->prev = leftHalfEdge;

            rightHalfEdge->face = newFace;
            rightHalfEdge->next = upperHalfEdge;
            rightHalfEdge->prev = bottomHalfEdge;

            upperHalfEdge->face = newFace;
            upperHalfEdge->next = leftHalfEdge;
            upperHalfEdge->prev = rightHalfEdge;
            upperHalfEdge->twin = upperHalfEdgeTwin;
            upperHalfEdgeTwin->twin = upperHalfEdge;

            leftHalfEdge->face = newFace;
            leftHalfEdge->next = bottomHalfEdge;
            leftHalfEdge->prev = upperHalfEdge;

            vf_new->halfedge = upperHalfEdge;
            vs_new->halfedge = leftHalfEdge;
        }

        Face* upperFace = structDS.addFace(upperTwins[0]);

        for (size_t i = 0; i < newVertices.size(); ++i) {
            upperTwins[i]->face = upperFace;
            upperTwins[i]->next = upperTwins[(i + 1) % newVertices.size()];
            upperTwins[i]->prev = upperTwins[(i + newVertices.size() - 1) % newVertices.size()];
            rightHalfEdges[i]->twin = leftHalfEdges[(i + 1) % newVertices.size()];
            leftHalfEdges[i]->twin = rightHalfEdges[(i + newVertices.size() - 1) % newVertices.size()];
        }

        structDS.removeFace(face);

        return upperFace;
    }


    Face* extrudeFace2(HalfedgeDS& structDS, Face* face, float distExtrude, float amountExtrude) {
        glm::vec3 faceNormal = face->getNormal(false);
        glm::vec3 faceCenter = face->calculateCenterPoint();

        auto faceVertexList = face->getVertices();
        auto faceHalfEdgeList = face->getHalfedges();

        float distanceToOrigin = glm::length(faceCenter); // Using glm::length to calculate the Euclidean distance to origin

        // Calculate the scaling factor: this could be any function of the distance that meets your needs
        // For example, we use a simple linear scale down with the distance
        float scaleFactor = 0.1f;
        float distExtrudeScale = std::max(0.0f, scaleFactor * distanceToOrigin);

        glm::vec3 newCenterPoint = faceCenter + faceNormal * static_cast<float>(distExtrude*distExtrudeScale);

        std::vector<glm::vec3> directionList;
        std::vector<Vertex*> newVertices;

        for (auto vertex : faceVertexList) {
            glm::vec3 displacement = vertex->position - faceCenter;
            directionList.push_back(displacement);
        }

        for (auto& displacement : directionList) {
            float displacementLength = glm::length(displacement);
            glm::vec3 displacementNormal = glm::normalize(displacement);
            float extrudeAmount = amountExtrude * displacementLength;
            glm::vec3 newPosition = newCenterPoint + displacementNormal * static_cast<float>(extrudeAmount);
            Vertex* newVertex = new Vertex(newPosition);
            structDS.addVertex(newVertex);
            newVertices.push_back(newVertex);
        }

        std::vector<Halfedge*> upperTwins, leftHalfEdges, rightHalfEdges;
        std::vector<Face*> newFaces;

        for (size_t i = 0; i < newVertices.size(); ++i) {
            size_t nextIndex = (i + 1) % newVertices.size();

            Vertex* vs = faceVertexList[i];
            Vertex* vf = faceVertexList[nextIndex];
            Vertex* vs_new = newVertices[i];
            Vertex* vf_new = newVertices[nextIndex];

            Halfedge* bottomHalfEdge = faceHalfEdgeList[i];

            Face* newFace = new Face(bottomHalfEdge);
            structDS.addFace(newFace);
            newFaces.push_back(newFace);

            Halfedge* rightHalfEdge = structDS.addHalfedge(vf);
            Halfedge* upperHalfEdge = structDS.addHalfedge(vf_new);
            Halfedge* upperHalfEdgeTwin = structDS.addHalfedge(vs_new);
            upperTwins.push_back(upperHalfEdgeTwin);
            Halfedge* leftHalfEdge = structDS.addHalfedge(vs_new);
            leftHalfEdges.push_back(leftHalfEdge);
            rightHalfEdges.push_back(rightHalfEdge);

            bottomHalfEdge->face = newFace;
            bottomHalfEdge->next = rightHalfEdge;
            bottomHalfEdge->prev = leftHalfEdge;

            rightHalfEdge->face = newFace;
            rightHalfEdge->next = upperHalfEdge;
            rightHalfEdge->prev = bottomHalfEdge;

            upperHalfEdge->face = newFace;
            upperHalfEdge->next = leftHalfEdge;
            upperHalfEdge->prev = rightHalfEdge;
            upperHalfEdge->twin = upperHalfEdgeTwin;
            upperHalfEdgeTwin->twin = upperHalfEdge;

            leftHalfEdge->face = newFace;
            leftHalfEdge->next = bottomHalfEdge;
            leftHalfEdge->prev = upperHalfEdge;

            vf_new->halfedge = upperHalfEdge;
            vs_new->halfedge = leftHalfEdge;
        }

        Face* upperFace =face;// structDS.addFace(upperTwins[0]);
        face->halfedge=upperTwins[0];

        for (size_t i = 0; i < newVertices.size(); ++i) {
            upperTwins[i]->face = upperFace;
            upperTwins[i]->next = upperTwins[(i + 1) % newVertices.size()];
            upperTwins[i]->prev = upperTwins[(i + newVertices.size() - 1) % newVertices.size()];
            rightHalfEdges[i]->twin = leftHalfEdges[(i + 1) % newVertices.size()];
            leftHalfEdges[i]->twin = rightHalfEdges[(i + newVertices.size() - 1) % newVertices.size()];
        }

        //face->setHalfedges(upperTwins); 


        return face;

    }

    


}