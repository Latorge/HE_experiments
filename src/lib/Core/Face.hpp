#ifndef FACE_HPP
#define FACE_HPP

#include <vector>
#include <memory>
#include "Struct.hpp"

class Face {
public:
    Halfedge* halfedge  =   nullptr;
    Face* parentFace    =   nullptr;
    glm::vec3 normal;
    bool normalFlag = false;
    glm::vec3 direction;
    glm::vec3 centerPoint;
    bool active = false;
    bool deleted = false;
    bool canGrowth = false;
    int operationCount = 0;
    int colorIndex = 0;
    bool defaultColor = true;
    bool fixColor = false;
    int objectIndex = 0;
    int objectGroupIndex = 0;
    int colorGroupIndex = 0;
    int numberVertices = 0;
    float area = 0;

    int id;
    std::vector<int> sub_id;

    Face(Halfedge* he, int _id=-1) ;

    glm::vec3 getNormal(bool useNormalFlag=false);
    void calculateNormal();

    glm::vec3 calculateCenterPoint() ;

    bool isFront(const glm::vec3& position) ;

    Halfedge* halfedgeFromPosition(const glm::vec3& position, float tolerance );
    Halfedge* halfedgeFromVertex(Vertex *vertex);

    Vertex* vertexFromPosition(const glm::vec3& position, float tolerance) ;
    bool hasVertex(Vertex* vertex);
    std::vector<Vertex*> getVertices();
    int calculateNumberVertex();
    std::vector<Halfedge*> getHalfedges();
    std::vector<Vertex*> commonVerticesWithVertex(Vertex *vertex);
    bool isNormalCalculated() const;
    // Additional methods would be translated similarly
};

#endif // FACE_HPP


