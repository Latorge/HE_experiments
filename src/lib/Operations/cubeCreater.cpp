#include "createOperations.hpp"

namespace Operations {
    ProtoMesh generateQuadArraysCube(int N, int M, int L, float cellSize) {

        float halfN = N / 2.0f;
        float halfM = M / 2.0f;
        float halfL = L / 2.0f;

        int NF = N + 1;
        int MF = M + 1;
        int LF = L + 1;

        ProtoMesh mesh;

        // Generate positions
        // Front plane -Z
        for (int i = 0; i < NF; i++) {
            for (int j = 0; j < MF; j++) {
                float x = (i - halfN) * cellSize;
                float y = (j - halfM) * cellSize;
                float z = -halfL * cellSize;
                mesh.positions.emplace_back(x, y, z);
            }
        }

        // Back plane +Z
        int backOffset = NF * MF;
        for (int i = 0; i < NF; i++) {
            for (int j = 0; j < MF; j++) {
                float x = (i - halfN) * cellSize;
                float y = (j - halfM) * cellSize;
                float z = halfL * cellSize;
                mesh.positions.emplace_back(x, y, z);
            }
        }

        // Left plane -X
        int leftOffset = 2 * backOffset;
        for (int i = 0; i < LF; i++) {
            for (int j = 0; j < MF; j++) {
                float x = -halfN * cellSize;
                float y = (j - halfM) * cellSize;
                float z = (i - halfL) * cellSize;
                mesh.positions.emplace_back(x, y, z);
            }
        }

        // Right plane +X
        int rightOffset = leftOffset + LF * MF;
        for (int i = 0; i < LF; i++) {
            for (int j = 0; j < MF; j++) {
                float x = halfN * cellSize;
                float y = (j - halfM) * cellSize;
                float z = (i - halfL) * cellSize;
                mesh.positions.emplace_back(x, y, z);
            }
        }

        // Top plane +Y
        int topOffset = rightOffset + LF * MF;
        for (int i = 0; i < NF; i++) {
            for (int j = 0; j < LF; j++) {
                float x = (i - halfN) * cellSize;
                float y = halfM * cellSize;
                float z = (j - halfL) * cellSize;
                mesh.positions.emplace_back(x, y, z);
            }
        }

        // Bottom plane -Y
        int bottomOffset = topOffset + NF * LF;
        for (int i = 0; i < NF; i++) {
            for (int j = 0; j < LF; j++) {
                float x = (i - halfN) * cellSize;
                float y = -halfM * cellSize;
                float z = (j - halfL) * cellSize;
                mesh.positions.emplace_back(x, y, z);
            }
        }

        // Generate cells for each plane
        auto generateCells = [&](int offset, int dim1, int dim2) {
            for (int i = 0; i < dim1 - 1; i++) {
                for (int j = 0; j < dim2 - 1; j++) {
                    int a = i * dim2 + j + offset;
                    int b = a + 1;
                    int c = a + dim2;
                    int d = c + 1;
                    mesh.cells.push_back({d, c, a, b});
                }
            }
        };

             // generate front plane cells -Z blue
        for (int i = 0; i < NF - 1; i++) {
            for (int j = 0; j < MF - 1; j++) {
                int a = i * MF + j;
                int b = a + 1;
                int c = a + MF;
                int d = c + 1;

                mesh.cells.push_back({d, c, a, b});
            }
        }

        // generate back plane cells +Z
        int cbackOffset = NF * MF;
        for (int i = 0; i < NF - 1; i++) {
            for (int j = 0; j < MF - 1; j++) {
                int a = i * MF + j + cbackOffset;
                int b = a + 1;
                int c = a + MF;
                int d = c + 1;

                mesh.cells.push_back({a, c, d, b});

            }
        }

        // generate left plane cells -X
        int cleftOffset = 2 * backOffset;
        for (int i = 0; i < LF - 1; i++) {
            for (int j = 0; j < MF - 1; j++) {
                int a = i * MF + j + cleftOffset;
                int b = a + 1;
                int c = a + MF;
                int d = c + 1;

                mesh.cells.push_back({d, b, a, c});
            }
        }

        // generate right plane cells +X
        int crightOffset = leftOffset + LF * MF;
        for (int i = 0; i < LF - 1; i++) {
            for (int j = 0; j < MF - 1; j++) {
                int a = i * MF + j + crightOffset;
                int b = a + 1;
                int c = a + MF;
                int d = c + 1;

                mesh.cells.push_back({a, b, d, c});
            }
        }

        // generate top plane cells
        int ctopOffset = rightOffset + LF * MF;
        for (int i = 0; i < NF - 1; i++) {
            for (int j = 0; j < LF - 1; j++) {
                int a = i * LF + j + ctopOffset;
                int b = a + 1;
                int c = a + LF;
                int d = c + 1;

                mesh.cells.push_back({d, c, a, b});
            }
        }

        // generate bottom plane cells
        int cbottomOffset = topOffset + NF * LF;
        for (int i = 0; i < NF - 1; i++) {
            for (int j = 0; j < LF - 1; j++) {
                int a = i * LF + j + cbottomOffset;
                int b = a + 1;
                int c = a + LF;
                int d = c + 1;

                //cells.push({a, b, d, c});
                mesh.cells.push_back({a, c, d, b});
            }
        }

/*
        generateCells(0, NF, MF);             // Front
        generateCells(backOffset, NF, MF);    // Back
        generateCells(leftOffset, LF, MF);    // Left
        generateCells(rightOffset, LF, MF);   // Right
        generateCells(topOffset, NF, LF);     // Top
        generateCells(bottomOffset, NF, LF);  // Bottom
*/

        return mesh;
    }
}


/*
             // generate front plane cells -Z blue
        for (int i = 0; i < NF - 1; i++) {
            for (int j = 0; j < MF - 1; j++) {
                int a = i * MF + j;
                int b = a + 1;
                int c = a + MF;
                int d = c + 1;

                mesh.cells.push_back({d, c, a, b});
            }
        }

        // generate back plane cells +Z
        int cbackOffset = NF * MF;
        for (int i = 0; i < NF - 1; i++) {
            for (int j = 0; j < MF - 1; j++) {
                int a = i * MF + j + cbackOffset;
                int b = a + 1;
                int c = a + MF;
                int d = c + 1;

                mesh.cells.push_back({a, c, d, b});

            }
        }

        // generate left plane cells -X
        int cleftOffset = 2 * backOffset;
        for (int i = 0; i < LF - 1; i++) {
            for (int j = 0; j < MF - 1; j++) {
                int a = i * MF + j + cleftOffset;
                int b = a + 1;
                int c = a + MF;
                int d = c + 1;

                mesh.cells.push_back({d, b, a, c});
            }
        }

        // generate right plane cells +X
        int crightOffset = leftOffset + LF * MF;
        for (int i = 0; i < LF - 1; i++) {
            for (int j = 0; j < MF - 1; j++) {
                int a = i * MF + j + crightOffset;
                int b = a + 1;
                int c = a + MF;
                int d = c + 1;

                mesh.cells.push_back({a, b, d, c});
            }
        }

        // generate top plane cells
        int ctopOffset = rightOffset + LF * MF;
        for (int i = 0; i < NF - 1; i++) {
            for (int j = 0; j < LF - 1; j++) {
                int a = i * LF + j + ctopOffset;
                int b = a + 1;
                int c = a + LF;
                int d = c + 1;

                mesh.cells.push_back({d, c, a, b});
            }
        }

        // generate bottom plane cells
        int cbottomOffset = topOffset + NF * LF;
        for (int i = 0; i < NF - 1; i++) {
            for (int j = 0; j < LF - 1; j++) {
                int a = i * LF + j + cbottomOffset;
                int b = a + 1;
                int c = a + LF;
                int d = c + 1;

                //cells.push({a, b, d, c});
                mesh.cells.push_back({a, c, d, b});
            }
        }


        struct VectorHash {
        size_t operator()(const glm::vec3& v) const {
            return std::hash<float>()(v.x * 1e4) ^ std::hash<float>()(v.y * 1e4) ^ std::hash<float>()(v.z * 1e4);
        }
    };

    struct VectorEqual {
        bool operator()(const glm::vec3& a, const glm::vec3& b) const {
            return std::abs(a.x - b.x) < 1e-4 && std::abs(a.y - b.y) < 1e-4 && std::abs(a.z - b.z) < 1e-4;
        }
    };

    ProtoMesh generateQuadArraysCube(int N, int M, int L, float cellSize) {
        int halfN = N / 2;
        int halfM = M / 2;
        int halfL = L / 2;

        int NF = N + 1;
        int MF = M + 1;
        int LF = L + 1;

        ProtoMesh mesh;
        std::unordered_map<glm::vec3, int, VectorHash, VectorEqual> vertexMap;

        auto addVertex = [&](const glm::vec3& vertex) -> int {
            if (vertexMap.find(vertex) == vertexMap.end()) {
                int index = mesh.positions.size();
                mesh.positions.push_back(vertex);
                vertexMap[vertex] = index;
                return index;
            }
            return vertexMap[vertex];
        };

        // Generate positions
        for (int i = 0; i < NF; i++) {
            for (int j = 0; j < MF; j++) {
                addVertex(glm::vec3((i - halfN) * cellSize, (j - halfM) * cellSize, -halfL * cellSize)); // Front plane -Z
                addVertex(glm::vec3((i - halfN) * cellSize, (j - halfM) * cellSize, halfL * cellSize));  // Back plane +Z
            }
        }

        for (int i = 0; i < LF; i++) {
            for (int j = 0; j < MF; j++) {
                addVertex(glm::vec3(-halfN * cellSize, (j - halfM) * cellSize, (i - halfL) * cellSize)); // Left plane -X
                addVertex(glm::vec3(halfN * cellSize, (j - halfM) * cellSize, (i - halfL) * cellSize));  // Right plane +X
            }
        }

        for (int i = 0; i < NF; i++) {
            for (int j = 0; j < LF; j++) {
                addVertex(glm::vec3((i - halfN) * cellSize, halfM * cellSize, (j - halfL) * cellSize)); // Top plane +Y
                addVertex(glm::vec3((i - halfN) * cellSize, -halfM * cellSize, (j - halfL) * cellSize)); // Bottom plane -Y
            }
        }

        // Generate cells for each plane
        auto generateCells = [&](int start, int dim1, int dim2) {
            int offset = start * dim1 * dim2;
            for (int i = 0; i < dim1 - 1; i++) {
                for (int j = 0; j < dim2 - 1; j++) {
                    int a = addVertex(mesh.positions[offset + i * dim2 + j]);
                    int b = addVertex(mesh.positions[offset + i * dim2 + j + 1]);
                    int c = addVertex(mesh.positions[offset + (i + 1) * dim2 + j]);
                    int d = addVertex(mesh.positions[offset + (i + 1) * dim2 + j + 1]);
                    mesh.cells.push_back({a, b, d, c});
                }
            }
        };

        generateCells(0, NF, MF);             // Front
        generateCells(1, NF, MF);             // Back
        generateCells(2, LF, MF);             // Left
        generateCells(2 + LF, LF, MF);        // Right
        generateCells(2 + 2 * LF, NF, LF);    // Top
        generateCells(2 + 2 * LF + NF, NF, LF); // Bottom

        return mesh;
    }
*/