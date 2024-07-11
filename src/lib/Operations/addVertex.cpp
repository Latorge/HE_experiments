#include "Operations.hpp"

namespace Operations {
    Vertex* addVertex(
        HalfedgeDS& structDS,
        const glm::vec3& position,
        bool checkDuplicates = false,
        float tolerance = 1e-10) {

        if (checkDuplicates) {
            for (auto& vertex : structDS.getVertices()) {
                if (vertex->matchesPosition(position, tolerance)) {
                    return vertex;
                }
            }
        }
        auto v=structDS.addVertex(position);
        v->subGroup = -1;
        return v;
    }

}
