#include "Modificators.hpp"

namespace Modificators {


void catmullClarkExp2(HalfedgeDS& structDS) {
    auto newPositions = calculatePosBorder(structDS);
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
            glm::vec3 faceCenter1 = faceVertexPos[halfEdge->face];
            glm::vec3 faceCenter2 = faceVertexPos[halfEdge->twin->face];
            edgePoint += faceCenter1 + faceCenter2;
            edgePoint /= 4.0f;
        } else {
            edgePoint /= 2.0f;
        }

        markedEdges.emplace_back(halfEdge, edgePoint);
    }

    for (auto halfEdgeInfo : markedEdges) {
        Operations::splitEdgeByPosition(structDS, halfEdgeInfo.first, halfEdgeInfo.second);
    }

    // Update vertex positions in the structure
    for (size_t i = 0; i < newPositions.size(); i++) {
        structDS.getVertices()[i]->position = newPositions[i];
    }

    // Reset visited flags
    for (auto halfedge : structDS.getHalfedges()) {
        halfedge->visited = false;
    }

    // Process faces with 8 vertices
    processFaces(structDS, 8);
    // Process faces with 6 vertices
    processFaces(structDS, 6);

    // Re-mark all halfedges as not visited
    for (auto halfedge : structDS.getHalfedges()) {
        halfedge->visited = false;
    }

    // Optional: re-calculate normals
    for (auto face : structDS.getFaces()) {
        face->normalFlag = false;
        face->calculateNormal();
    }
}

void processFaces(HalfedgeDS& structDS, int vertexCount) {
    std::vector<Face*> markedFaces;
    for (auto face : structDS.getFaces()) {
        if (face->calculateNumberVertex() == vertexCount) {
            markedFaces.push_back(face);
        }
    }

    std::vector<Halfedge*> newHalfEdges;
    for (auto face : markedFaces) {
        std::vector<Vertex*> newVertices;
        Halfedge* currentHalfedge = face->halfedge;
        int vertexIndex = 0;
        do {
            int neededIndices[] = {1, 5};  // Default for 8 vertices
            if (vertexCount == 6) {
                neededIndices[0] = 1; neededIndices[1] = 4;  // Adjust for 6 vertices
            }
            if (!currentHalfedge->visited && (vertexIndex == neededIndices[0] || vertexIndex == neededIndices[1])) {
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

    for (auto halfEdgeInfo : newHalfEdges) {
        Operations::splitEdge(structDS,halfEdgeInfo);  // Assuming splitEdge takes a Halfedge and a position ratio
    }
}

    void catmullClarkExp(HalfedgeDS& structDS) {
        auto newPositions =  calculatePosBorder(structDS);
        auto faceVertexPos = calculateFaceVerticesPos(structDS);
        //auto edgeVertexPos = calculateEdgeVerticesPos(structDS);

        int vlen =structDS.getVertices().size();
 /*
        std::vector<std::pair<Halfedge*, glm::vec3>> markedEdges;
        for (auto halfEdge : structDS.getHalfedges()) {

            glm::vec3 edgePoint = halfEdge->vertex->position;
            edgePoint +=halfEdge->twin->vertex->position;

            if (halfEdge->face && halfEdge->twin->face)
             {
                glm::vec3 faceCenter1 = faceVertexPos[halfEdge->face];
                glm::vec3 faceCenter2 = faceVertexPos[halfEdge->twin->face];
                edgePoint += faceCenter1 + faceCenter2;
                edgePoint /= 4.0f;
            } else {
                edgePoint /= 2.0f;
            }

            markedEdges.push_back(std::make_pair(halfEdge, edgePoint));
        }

*/
 // Update vertex positions
       

        std::vector<std::pair<Halfedge*, glm::vec3>> markedEdges;
        std::unordered_set<Halfedge*> visitedHalfedges;  // To track which halfedges have been processed

        for (auto& halfEdge : structDS.getHalfedges()) {
            // Check if halfEdge or its twin has been visited
            if (visitedHalfedges.find(halfEdge) == visitedHalfedges.end()
                // &&
                //visitedHalfedges.find(halfEdge->twin) == visitedHalfedges.end()

                )
            {
                // If neither the halfEdge nor its twin has been visited, process it

                glm::vec3 edgePoint = halfEdge->vertex->position;
                edgePoint += halfEdge->twin->vertex->position;

                // If halfEdge is connected to a face and so is its twin
                if (halfEdge->face && halfEdge->twin->face) {
                    glm::vec3 faceCenter1 = faceVertexPos[halfEdge->face];//halfEdge->face->calculateCenterPoint();
                    glm::vec3 faceCenter2 = faceVertexPos[halfEdge->twin->face];//halfEdge->twin->face->calculateCenterPoint();
                    edgePoint += faceCenter1 + faceCenter2;
                    edgePoint /= 4.0f;
                } else {
                    edgePoint /= 2.0f;
                }

                markedEdges.emplace_back(halfEdge, edgePoint);
                visitedHalfedges.insert(halfEdge);
               // visitedHalfedges.insert(halfEdge->twin);
            }
        }


        for (auto halfEdgeInfo : markedEdges) {
            Operations::splitEdgeByPosition(structDS, halfEdgeInfo.first, halfEdgeInfo.second);
           // Operations::splitEdge(structDS, halfEdgeInfo.first);
        }

        for (size_t i = 0; i < vlen; ++i) {
            structDS.getVertices()[i]->position = newPositions[i];
        }

        // Reset visited flags
        for (auto halfedge : structDS.getHalfedges()) {
            halfedge->visited = false;
        }
/*
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

*/
         //struct.loops();  // Assuming this function does some internal restructuring or cleanup

         std::vector<Face*> selectedFaces;

        // Collect faces with 8 vertices
        for (auto& face : structDS.getFaces()) {
            if (face->calculateNumberVertex() == 8) {
                selectedFaces.push_back(face);
            }
        }

        float ratio = 0.5f;
        std::vector<Halfedge*> newHalfEdges;
        markedEdges.clear();

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
                markedEdges.emplace_back(newHalfEdge, faceVertexPos[face]);
            }
        }

        // Split edges at calculated mid-points
        for (auto halfEdge : markedEdges) {
            //Operations::splitEdgeRatio(structDS, halfEdge, ratio);
            Operations::splitEdgeByPosition(structDS, halfEdge.first, halfEdge.second);
        }


        //struct.loops();  // Assuming this function does some internal restructuring or cleanup

        selectedFaces.clear();
        for (auto& face : structDS.getFaces()) {
            if (face->calculateNumberVertex() == 6) {
                selectedFaces.push_back(face);
            }
        }

        newHalfEdges.clear();
        markedEdges.clear();

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


        for (auto face : structDS.getFaces()) {
            face->normalFlag = false;
            face->calculateNormal();
        }


    }


    bool areVectorsEqual(const glm::vec3& v1, const glm::vec3& v2, float epsilon) {
        return glm::all(glm::epsilonEqual(v1, v2, epsilon));
    }

    void compareAndUpdate(glm::vec3& borderVert, const glm::vec3& vertPoint, const glm::vec3& alternative, float tolerance) {
        if (areVectorsEqual(borderVert, vertPoint, tolerance)) {
            borderVert = alternative;
        }
    }

  

    std::vector<glm::vec3> calculatePosBorder(HalfedgeDS &structDS)
    {
        const std::vector<Vertex*>  vertices = structDS.getVertices();
        size_t vlen = vertices.size();
        
        // Copy existing positions
        std::vector<glm::vec3> newPositions;
        //newPositions.reserve(vlen);
        for (auto vertex : vertices)
        {
            newPositions.emplace_back(vertex->position);
        }

        // Coefficients map
        /*
        std::unordered_map<int, glm::vec3> kmap;
        for (int i = 1; i <= 32; ++i)
        { // Start from 1 to avoid division by zero
            float beta = 3.0f / (2.0f * i);
            float rho = 1.0f / (4.0f * i);
            kmap[i] = glm::vec3(1.0f - beta - rho, beta / i, rho / i);
        }
        */      

        const float coeff6d8 = 6.0f / 8.0f;
        const float coeff1d8 = 1.0f / 8.0f;


        for (size_t i = 0; i < vlen; ++i)
        {
            Vertex* vertex = vertices[i];
            std::vector<Halfedge*> neighbors = vertex-> allHalfedgesInLoopExp();
            size_t nlen = neighbors.size();

            std::vector<Halfedge*> borderEdges;

            Halfedge* startHalfEdge = vertex->halfedge;
           // if (!startHalfEdge)
            //    continue;

            Face* face = startHalfEdge->face ? startHalfEdge->face : startHalfEdge->twin->face;
            if (!face)
                continue;

            for (auto currentHalfedge : neighbors)
            {
                if (!currentHalfedge->face || !currentHalfedge->twin->face){
                    borderEdges.push_back(currentHalfedge);
                }
            }

            glm::vec3 vertPoint = vertex->position;

            if (borderEdges.size()== 1 || nlen == 1)
            {
                newPositions[i] = vertPoint;
            }
            else if (borderEdges.size() >= 2)
            {
                float tolerance = 0.0000001f; 
                /*
                glm::vec3 borderVert0 = borderEdges[0]->vertex->position;
                if (borderVert0 == vertPoint)
                    borderVert0 = borderEdges[0]->twin->vertex->position;

                glm::vec3 borderVert1 = borderEdges[1]->twin->vertex->position;
                if (borderVert1 == vertPoint)
                    borderVert1 = borderEdges[1]->vertex->position;
                    */
/*
                glm::vec3 borderVert0 = borderEdges[0]->vertex->position;
                compareAndUpdate(borderVert0, vertPoint, borderEdges[0]->twin->vertex->position, tolerance);
                glm::vec3 borderVert1 = borderEdges[1]->twin->vertex->position;
                compareAndUpdate(borderVert1, vertPoint, borderEdges[1]->vertex->position, tolerance);
*/
/**/
                glm::vec3 borderVert0 = borderEdges[0]->vertex->position;
                glm::vec3 borderVert1 = borderEdges[1]->twin->vertex->position;

                if (glm::all(glm::epsilonEqual(borderVert0, vertPoint, glm::vec3(0.000001f))))
                    borderVert0 = borderEdges[0]->twin->vertex->position;
                if (glm::all(glm::epsilonEqual(borderVert1, vertPoint, glm::vec3(0.000001f))))
                    borderVert1 = borderEdges[1]->vertex->position;

                vertPoint   *= coeff6d8;
                borderVert0 *= coeff1d8;
                borderVert1 *= coeff1d8;

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
                    Face* face2  = currentHalfedge->face ? currentHalfedge->face : currentHalfedge->twin->face;
                    if (currentHalfedge->face)
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
            glm::vec3 faceVertexPos(0.0f);  // Initialize a zero vector

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


    std::vector<glm::vec3> calculatePosBorder2(HalfedgeDS &structDS)
    {
        const std::vector<Vertex*>  vertices = structDS.getVertices();
        size_t vlen = vertices.size();
        
        // Copy existing positions
        std::vector<glm::vec3> newPositions;
        //newPositions.reserve(vlen);
        for (auto vertex : vertices)
        {
            newPositions.emplace_back(vertex->position);
        }

        // Coefficients map
        /*
        std::unordered_map<int, glm::vec3> kmap;
        for (int i = 1; i <= 32; ++i)
        { // Start from 1 to avoid division by zero
            float beta = 3.0f / (2.0f * i);
            float rho = 1.0f / (4.0f * i);
            kmap[i] = glm::vec3(1.0f - beta - rho, beta / i, rho / i);
        }
        */      

        const float coeff6d8 = 6.0f / 8.0f;
        const float coeff1d8 = 1.0f / 8.0f;


        for (size_t i = 0; i < vlen; ++i)
        {
            Vertex* vertex = vertices[i];
            std::vector<Halfedge*> neighbors = vertex->allHalfedgesInLoop();
            size_t nlen = neighbors.size();

            std::vector<Halfedge*> borderEdges;

            Halfedge* startHalfEdge = vertex->halfedge;
            if (!startHalfEdge)
                continue;

            Face* face = startHalfEdge->face ? startHalfEdge->face : startHalfEdge->twin->face;
            if (!face)
                continue;

            for (auto currentHalfedge : neighbors)
            {
                if (!currentHalfedge->face || !currentHalfedge->twin->face){
                    borderEdges.push_back(currentHalfedge);
                }
            }

            glm::vec3 vertPoint = vertex->position;

            if (borderEdges.size()== 1 || nlen == 1)
            {
                newPositions[i] = vertPoint;
            }
            else if (borderEdges.size() >= 2)
            {
                float tolerance = 0.0000001f; 
                /*
                glm::vec3 borderVert0 = borderEdges[0]->vertex->position;
                if (borderVert0 == vertPoint)
                    borderVert0 = borderEdges[0]->twin->vertex->position;

                glm::vec3 borderVert1 = borderEdges[1]->twin->vertex->position;
                if (borderVert1 == vertPoint)
                    borderVert1 = borderEdges[1]->vertex->position;
                    */
/*
                glm::vec3 borderVert0 = borderEdges[0]->vertex->position;
                compareAndUpdate(borderVert0, vertPoint, borderEdges[0]->twin->vertex->position, tolerance);
                glm::vec3 borderVert1 = borderEdges[1]->twin->vertex->position;
                compareAndUpdate(borderVert1, vertPoint, borderEdges[1]->vertex->position, tolerance);
*/
/**/
                glm::vec3 borderVert0 = borderEdges[0]->vertex->position;
                glm::vec3 borderVert1 = borderEdges[1]->twin->vertex->position;

                if (glm::all(glm::epsilonEqual(borderVert0, vertPoint, glm::vec3(0.000001f))))
                    borderVert0 = borderEdges[0]->twin->vertex->position;
                if (glm::all(glm::epsilonEqual(borderVert1, vertPoint, glm::vec3(0.000001f))))
                    borderVert1 = borderEdges[1]->vertex->position;

                vertPoint   *= coeff6d8;
                borderVert0 *= coeff1d8;
                borderVert1 *= coeff1d8;

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
                    Face* face2  = currentHalfedge->face ? currentHalfedge->face : currentHalfedge->twin->face;
                    if (currentHalfedge->face)
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


}   