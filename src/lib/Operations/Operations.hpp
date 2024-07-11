// Operations.hpp
#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include "../Core/Vertex.hpp"
#include "../Core/Halfedge.hpp"
#include "../Core/Face.hpp"
#include "../Core/HalfedgeDS.hpp"

namespace Operations {
    Vertex* addVertex(int vertexData);  // Just an example signature
    int addFace(int faceData);
    Halfedge* addEdge(HalfedgeDS& structDS,Vertex* v1,Vertex* v2, bool allowParallels = false);
}

#endif // OPERATIONS_HPP
