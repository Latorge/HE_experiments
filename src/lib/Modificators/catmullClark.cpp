#include "Modificators.hpp"

namespace Modificators {

    void catmullClarkExp(HalfedgeDS& structDS) {
        auto newPositions =  calculatePosBorder(structDS);
        auto faceVertexPos = calculateFaceVerticesPos(structDS);
        auto edgeVertexPos = calculateEdgeVerticesPos(structDS);

        // Initialize all halfedges as not visited
        for (auto halfedge : structDS.getHalfedges()) {
            halfedge->visited = false;
        }

        std::vector<std::pair<Halfedge*, glm::vec3>> markedEdges;
        for (auto halfEdge : structDS.getHalfedges()) {
            halfEdge->visited = true;
            halfEdge->twin->visited = true;

            glm::vec3 edgePoint = halfEdge->vertex->position + halfEdge->twin->vertex->position;

            if (halfEdge->face && halfEdge->twin->face) {
                glm::vec3 faceCenter1 = faceVertexPos.at(halfEdge->face);
                glm::vec3 faceCenter2 = faceVertexPos.at(halfEdge->twin->face);
                edgePoint += faceCenter1 + faceCenter2;
                edgePoint /= 4.0f;
            } else {
                edgePoint /= 2.0f;
            }

            markedEdges.push_back(std::make_pair(halfEdge, edgePoint));
        }

        for (auto halfEdgeInfo : markedEdges) {
            Operations::splitEdgeByPosition(structDS, halfEdgeInfo.first, halfEdgeInfo.second);
        }

        // Update vertex positions
        for (size_t i = 0; i < structDS.getVertices().size(); ++i) {
            structDS.getVertices()[i]->position = newPositions[i];
        }

        // Reset visited flags
        for (auto halfedge : structDS.getHalfedges()) {
            halfedge->visited = false;
        }

        std::vector<Face*> markedFaces;
        for (auto face : structDS.getFaces()) {
            if (face->getVertices().size()== 8) {
                markedFaces.push_back(face);
            }
        }

        std::vector<std::pair<Halfedge*, glm::vec3>> newHalfEdges;
        for (auto face : markedFaces) {
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

            Halfedge* newHalfEdge = Operations::cutFace(structDS,face, newVertices[0], newVertices[1], true);
            newHalfEdges.push_back(std::make_pair(newHalfEdge, faceVertexPos[face]));
        }

        for (auto halfEdgeInfo : newHalfEdges) {
            Operations::splitEdgeByPosition(structDS,halfEdgeInfo.first, halfEdgeInfo.second);
        }

        // Reset visited flags
        for (auto halfedge : structDS.getHalfedges()) {
            halfedge->visited = false;
        }

        for (auto face : structDS.getFaces()) {
            face->normalFlag = false;
            face->calculateNormal();
        }
    }

  

    std::vector<glm::vec3> calculatePosBorder(HalfedgeDS &structDS)
    {
        const std::vector<Vertex*>  vertices = structDS.getVertices();
        size_t vlen = vertices.size();

        // Copy existing positions
        std::vector<glm::vec3> newPositions;
        newPositions.reserve(vlen);
        for (auto vertex : vertices)
        {
            newPositions.push_back(vertex->position);
        }

        // Coefficients map
        std::unordered_map<int, glm::vec3> kmap;
        for (int i = 1; i <= 32; ++i)
        { // Start from 1 to avoid division by zero
            float beta = 3.0f / (2.0f * i);
            float rho = 1.0f / (4.0f * i);
            kmap[i] = glm::vec3(1.0f - beta - rho, beta / i, rho / i);
        }

        for (size_t i = 0; i < vlen; ++i)
        {
            Vertex* vertex = vertices[i];
            std::vector<Halfedge*> neighbors = vertex->allHalfedgesInLoop();
            size_t nlen = neighbors.size();

            std::vector<Halfedge*> borderEdges;

            Halfedge *startHalfEdge = vertex->halfedge;
            if (!startHalfEdge)
                continue;
            Face* face = startHalfEdge->face ? startHalfEdge->face : startHalfEdge->twin->face;
            if (!face)
                continue;

            for (auto currentHalfedge : neighbors)
            {
                if (!currentHalfedge->face || !currentHalfedge->twin->face)
                {
                    borderEdges.push_back(currentHalfedge);
                }
            }

            glm::vec3 vertPoint = vertex->position;

            if (borderEdges.size() == 1 || nlen == 1)
            {
                newPositions[i] = vertPoint;
            }
            else if (borderEdges.size() == 2)
            {
                glm::vec3 borderVert0 = borderEdges[0]->vertex->position;
                if (borderVert0 == vertPoint)
                    borderVert0 = borderEdges[0]->twin->vertex->position;

                glm::vec3 borderVert1 = borderEdges[1]->twin->vertex->position;
                if (borderVert1 == vertPoint)
                    borderVert1 = borderEdges[1]->vertex->position;

                vertPoint   *= 6.0f / 8.0f;
                borderVert0 *= 1.0f / 8.0f;
                borderVert1 *= 1.0f / 8.0f;

                vertPoint += borderVert0;
                vertPoint += borderVert1;
                newPositions[i] = vertPoint;
            }
            else
            {
                glm::vec3 neighborSum(0.0f);
                glm::vec3 faceSum(0.0f);

                for (auto currentHalfedge : neighbors)
                {
                    neighborSum += currentHalfedge->twin->vertex->position;
                    Face* face2 = currentHalfedge->face ? currentHalfedge->face : currentHalfedge->twin->face;
                    if (face2)
                        faceSum += face2->calculateCenterPoint();
                }

                float baseScalar = (nlen - 2.0f) / nlen;
                float neighborScalar = 1.0f / (nlen * nlen);

                vertPoint   *= baseScalar;
                neighborSum *= neighborScalar;
                faceSum     *= neighborScalar;

                vertPoint += neighborSum;
                vertPoint += faceSum;
                newPositions[i] = vertPoint;
            }
        }
        return newPositions;
    }


    std::unordered_map<Face*, glm::vec3> calculateFaceVerticesPos(HalfedgeDS& structDS) {
        std::unordered_map<Face*, glm::vec3> faceVerticesPosHash;

        for (auto face : structDS.getFaces()) {
            std::vector<Vertex*> faceVertices = face->getVertices();
            size_t vlen = faceVertices.size();
            glm::vec3 faceVertexPos(0.0f, 0.0f, 0.0f);  // Initialize a zero vector

            for (auto vertex : faceVertices) {
                faceVertexPos += vertex->position;
            }

            if (vlen > 0) {
                faceVertexPos /= static_cast<float>(vlen);  // Divide by the number of vertices
            }

            faceVerticesPosHash[face] = faceVertexPos;
        }

        return faceVerticesPosHash;
    }


    std::unordered_map<Halfedge*, glm::vec3> calculateEdgeVerticesPos(HalfedgeDS& structDS) {
        std::unordered_map<Halfedge*, glm::vec3> edgeVerticesPosHash;

        // Mark all halfedges as not visited
        for (auto halfedge : structDS.getHalfedges()) {
            halfedge->visited = false;
        }

        for (auto edge : structDS.getHalfedges()) {
            if (edge->visited) continue;
            edge->visited = true;
            edge->twin->visited = true;

            glm::vec3 edgeVertexPos(0.0f);

            // Initialize the edge position in the hash map
            edgeVerticesPosHash[edge] = edgeVertexPos;

            if (edge->face == nullptr) {
                glm::vec3 heVertexPos = edge->vertex->position * (6.0f / 16.0f);
                edgeVertexPos += heVertexPos;

                glm::vec3 heVertexPos0 = edge->next->next->vertex->position;
                glm::vec3 heVertexPos1 = edge->next->next->next->vertex->position;

                edgeVertexPos += heVertexPos0 * (1.0f / 16.0f);
                edgeVertexPos += heVertexPos1 * (1.0f / 16.0f);
            }

            Halfedge* hef = edge->twin;
            if (hef->face == nullptr) {
                glm::vec3 hefVertexPos = hef->vertex->position * (6.0f / 16.0f);
                edgeVertexPos += hefVertexPos;

                glm::vec3 hefVertexPos0 = hef->next->next->vertex->position;
                glm::vec3 hefVertexPos1 = hef->next->next->next->vertex->position;

                edgeVertexPos += hefVertexPos0 * (1.0f / 16.0f);
                edgeVertexPos += hefVertexPos1 * (1.0f / 16.0f);
            }

            // Store the computed position
            edgeVerticesPosHash[edge] = edgeVertexPos;
        }

        // Reset the visited flags
        for (auto halfedge : structDS.getHalfedges()) {
            halfedge->visited = false;
        }

        return edgeVerticesPosHash;
    }



}   