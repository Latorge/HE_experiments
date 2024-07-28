// createOperations.hpp
#ifndef MODIFICATORS_HPP
#define MODIFICATORS_HPP

#include "../Operations/Operations.hpp"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <glm/gtc/epsilon.hpp>  

namespace Modificators {

    void catmullClarkExp(HalfedgeDS &structDS);

    void processFaces(HalfedgeDS &structDS, int vertexCount);

    void catmullClarkExp2(HalfedgeDS &structDS);

    bool areVectorsEqual(const glm::vec3 &v1, const glm::vec3 &v2, float epsilon);
    void compareAndUpdate(glm::vec3 &borderVert, const glm::vec3 &vertPoint, const glm::vec3 &alternative, float tolerance);

    std::vector<glm::vec3> calculatePosBorder(HalfedgeDS &structDS);
    std::unordered_map<Face*, glm::vec3> calculateFaceVerticesPos(HalfedgeDS &structDS);
    std::unordered_map<Halfedge*, glm::vec3> calculateEdgeVerticesPos(HalfedgeDS &structDS);

    std::vector<glm::vec3> calculatePosBorder2(HalfedgeDS &structDS);

    void quadSubDivideStruct(HalfedgeDS &structDS);
    void quadSubDivide(HalfedgeDS &structDS);

    Face *extrudeFace(HalfedgeDS &structDS, Face *face, float distExtrude, float amountExtrude);
    Face *extrudeFace2(HalfedgeDS &structDS, Face *face, float distExtrude, float amountExtrude);
}

#endif // MODIFICATORS_HPP
