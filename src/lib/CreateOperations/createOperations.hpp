// createOperations.hpp
#ifndef CREATEOPERATIONS_HPP
#define CREATEOPERATIONS_HPP

#include "../Operations/Operations.hpp"

namespace Operations {

    void setFromGeometry(HalfedgeDS& structure,const std::vector<glm::vec3>& positions,const std::vector<std::vector<int>>& cells, double tolerance = 1e-10);

    std::string getPositionHash(const glm::vec3 &position, double shiftMultiplier);
    ProtoMesh computeUniquePositionsArray(const std::vector<glm::vec3> &positions, const std::vector<std::vector<int>> &cells, double tolerance = 1e-7);

    ProtoMesh generateQuadArraysCube(int N, int M, int L, float cellSize);
    ProtoMesh generateQuadArrays(int N, int M, float cellSize);

    ProtoMesh generateTube(std::vector<glm::vec3> sourcePoints, int numSegments, float startAngle, float endAngle);

    ProtoMesh generateTubeNL(std::vector<glm::vec3> sourcePoints, float radius, int numSegments, float startAngle, float endAngle);
}

#endif // CREATEOPERATIONS_HPP
