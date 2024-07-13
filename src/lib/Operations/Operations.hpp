// Operations.hpp
#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <glm/glm.hpp>
#include <string>


#include "../Core/Vertex.hpp"
#include "../Core/Halfedge.hpp"
#include "../Core/Face.hpp"
#include "../Core/HalfedgeDS.hpp"


namespace Operations {

    struct ProtoMesh {
        std::vector<glm::vec3> positions;
        std::vector<std::vector<int>> cells;
    };

    Vertex* addVertex(HalfedgeDS& structDS, const glm::vec3& position,bool checkDuplicates,float tolerance= 1e-10);
    Face*     addFace(HalfedgeDS& structure, std::vector<Halfedge*>& halfedges);
    Halfedge* addEdge(HalfedgeDS& structDS, Vertex* v1,Vertex* v2, bool allowParallels = false);
    bool makeHalfedgesAdjacent(Halfedge* halfIn, Halfedge* halfOut);
   
/*
    void setFromGeometry(HalfedgeDS& structure,const std::vector<glm::vec3>& positions,const std::vector<std::vector<int>>& cells, double tolerance = 1e-10);

    std::string getPositionHash(const glm::vec3 &position, double shiftMultiplier);
    ProtoMesh computeUniquePositionsArray(const std::vector<glm::vec3> &positions, const std::vector<std::vector<int>> &cells, double tolerance = 1e-7);

    ProtoMesh generateQuadArraysCube(int N, int M, int L, float cellSize);
    */

}

#endif // OPERATIONS_HPP
