#include "createOperations.hpp"
#include <unordered_map>

namespace Operations {
    void setFromGeometry(
        HalfedgeDS& structure,
        const std::vector<glm::vec3>& positions,
        const std::vector<std::vector<int>>& cells,
        double tolerance) {

        structure.clear();

        // Maps to store vertices and halfedges
        std::unordered_map<int, Vertex*> vertexMap;
        std::unordered_map<std::string, Halfedge*> halfedgeMap;

        for (size_t faceIndex = 0; faceIndex < cells.size(); faceIndex++) {
            std::vector<Halfedge*> loopHalfedges(cells[faceIndex].size(), nullptr);

            for (size_t i = 0; i < cells[faceIndex].size(); i++) {
                int i1 = cells[faceIndex][i];
                int i2 = cells[faceIndex][(i + 1) % cells[faceIndex].size()];
                
                // Ensure vertices exist
                Vertex* v1 = nullptr;
                if (vertexMap.find(i1) == vertexMap.end()) {
                    v1 = structure.addVertex(positions[i1], true);
                    vertexMap[i1] = v1;
                } else {
                    v1 = vertexMap[i1];
                }

                Vertex* v2 = nullptr;
                if (vertexMap.find(i2) == vertexMap.end()) {
                    v2 = structure.addVertex(positions[i2],true);
                    vertexMap[i2] = v2;
                } else {
                    v2 = vertexMap[i2];
                }

                // Create halfedges if they don't exist
                std::string hash1 = std::to_string(i1) + "-" + std::to_string(i2);
                Halfedge* h1 = nullptr;
                /*
                if (halfedgeMap.find(hash1) == halfedgeMap.end()) {
                    h1 = new Halfedge(v1);
                    Halfedge* h2 = new Halfedge(v2);

                    // Set mutual twins
                    h1->twin = h2;
                    h2->twin = h1;

                    structure.addHalfedge(h1);
                    structure.addHalfedge(h2);

                    halfedgeMap[hash1] = h1;
                    halfedgeMap[std::to_string(i2) + "-" + std::to_string(i1)] = h2;
                } else {
                    h1 = halfedgeMap[hash1];
                }
*/
                if (halfedgeMap.find(hash1) == halfedgeMap.end()) {
                    h1 = Operations::addEdge(structure, v1, v2);
                    Halfedge* h2 = h1->twin;
                    
                    halfedgeMap[hash1] = h1;
                    halfedgeMap[std::to_string(i2) + "-" + std::to_string(i1)] = h2;
                } else {
                    h1 = halfedgeMap[hash1];
                }

                loopHalfedges[i] = h1;
            }

            // Create the face from the loop of halfedges
            //Face* face = new Face(loopHalfedges.front(), structure.getNextFaceID());
            Face* face = Operations::addFace(structure,loopHalfedges);
            //structure.addFace(face);
            for (Halfedge* he : loopHalfedges) {
                he->face = face;
            }
        }
    }


    std::string getPositionHash(const glm::vec3& position, double shiftMultiplier) {
        int x = std::round(position.x * shiftMultiplier);
        int y = std::round(position.y * shiftMultiplier);
        int z = std::round(position.z * shiftMultiplier);
        return std::to_string(x) + std::to_string(y) + std::to_string(z);
    }

    ProtoMesh computeUniquePositionsArray(
        const std::vector<glm::vec3>& positions,
        const std::vector<std::vector<int>>& cells,
        double tolerance) {

        double decimalShift = std::log10(1 / tolerance);
        double shiftMultiplier = std::pow(10, decimalShift);

        std::unordered_map<std::string, int> hashMap;
        ProtoMesh resultMesh;
        int vertexCounter = 0;

        for (const auto& position : positions) {
            std::string hash = getPositionHash(position, shiftMultiplier);

            if (hashMap.find(hash) == hashMap.end()) {
                hashMap[hash] = vertexCounter++;
                resultMesh.positions.push_back(position);
            }
        }

        for (const auto& cell : cells) {
            std::vector<int> reindexedCell;
            for (int vertexIndex : cell) {
                glm::vec3 originalPosition = positions[vertexIndex];
                std::string hash = getPositionHash(originalPosition, shiftMultiplier);

                auto foundIt = hashMap.find(hash);
                if (foundIt != hashMap.end()) {
                    reindexedCell.push_back(foundIt->second);
                } else {
                    std::cout << "Warning: No unique index found for hash '" << hash << "'\n";
                    reindexedCell.push_back(-1); // Error index if not found
                }
            }
            resultMesh.cells.push_back(reindexedCell);
        }
        return resultMesh;
    }
}
