#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <glm/glm.hpp>
#include <set>
#include "Struct.hpp"

class Vertex {
public:
    glm::vec3 position;
    Halfedge* halfedge  =   nullptr;
    bool needUpdate     =   false;
    int subGroup        = -1;
    int id;

    Vertex(int& _id);
    Vertex(float x, float y, float z, int& _id);
    Vertex(glm::vec3 vec, int& _id);

    void changeComponent(float x, float y, float z) ;
    void change(const glm::vec3& vector) ;
    void move(const glm::vec3& vector) ;

    std::vector<Halfedge*> collectFreeHalfedges() ;
    Halfedge *freeHalfedgesInLoop();
    std::vector<Halfedge *> collectBoundaryHalfedges();
    bool isFree() ;
    bool isIsolated();

    std::vector<Face*> commonFacesWithVertex(Vertex* other) ;
    bool matchesPosition(const glm::vec3& pos, float tolerance ) ;

    Halfedge* getHalfedgeToVertex(Vertex* other);
    bool isConnectedToVertex(Vertex* other);
    std::vector<Halfedge*> loopCW() ;
   // glm::vec3 calculateVertexNormal() ;
private:
    float calculateAngleWeight(Face* face, const glm::vec3& faceNormal, size_t numCommonVertices) ;
};



#endif // VERTEX_HPP
