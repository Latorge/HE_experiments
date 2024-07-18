// createOperations.hpp
#ifndef MODIFICATORS_HPP
#define MODIFICATORS_HPP

#include "../Operations/Operations.hpp"

#include <vector>
#include <unordered_map>

namespace Modificators {

    void catmullClarkExp(HalfedgeDS &structDS);

    std::vector<glm::vec3> calculatePosBorder(HalfedgeDS &structDS);
    std::unordered_map<Face*, glm::vec3> calculateFaceVerticesPos(HalfedgeDS &structDS);
    std::unordered_map<Edge*, glm::vec3> calculateEdgeVerticesPos(HalfedgeDS &structDS);


    void quadSubDivideStruct(HalfedgeDS &structDS);
    void quadSubDivide(HalfedgeDS &structDS);
}

#endif // MODIFICATORS_HPP
