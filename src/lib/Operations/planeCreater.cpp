#include "createOperations.hpp"
#include <vector>


namespace Operations {
    ProtoMesh generateQuadArrays(int N, int M, float cellSize) {
        int NC = N + 1;
        int MC = M + 1;
        float halfN = static_cast<float>(NC) / 2.0f;
        float halfM = static_cast<float>(MC) / 2.0f;

        ProtoMesh mesh;

        // Generate positions
        for (int i = 0; i < NC; i++) {
            for (int j = 0; j < MC; j++) {
                float x = (i - halfN) * cellSize;
                float y = (j - halfM) * cellSize;
                float z = 0.0f;

                mesh.positions.push_back(glm::vec3(x, y, z));
            }
        }

        // Generate cells
        for (int i = 0; i < NC - 1; i++) {
            for (int j = 0; j < MC - 1; j++) {
                int a = i * MC + j;
                int b = a + 1;
                int c = a + MC;
                int d = c + 1;

                mesh.cells.push_back({b, a, c, d});
            }
        }

        return mesh;
    }
}
