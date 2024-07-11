#ifndef HALFEDGE_DS_H
#define HALFEDGE_DS_H

#include <vector>
#include "Vertex.hpp"
#include "Halfedge.hpp"
#include "Face.hpp"

class HalfedgeDS {
private:
    std::vector<Vertex*> vertices;
    std::vector<Halfedge*> halfedges;
    std::vector<Face*> faces;

    // ID counters for each type of component
    int vertexIDCounter = 0;
    int halfedgeIDCounter = 0;
    int faceIDCounter = 0;
/*
    int addHalfedge(const Halfedge& halfedge) {
        Halfedge* newHalfedge = new Halfedge(halfedge, halfedgeIDCounter); 
        halfedges.push_back(newHalfedge);
        return halfedgeIDCounter++;  // Return the ID then increment the counter
    }

    int addFace(const Face& face) {
        Face* newFace = new Face(face, faceIDCounter); 
        faces.push_back(newFace);
        return faceIDCounter++;  // Return the ID then increment the counter
    }
*/
public:

    // Methods to add components to the data structure
    Vertex* addVertex(glm::vec3 vec3) {
        Vertex* newVertex = new Vertex(vec3, vertexIDCounter); 
        vertices.push_back(newVertex);
        vertexIDCounter++;
        return newVertex; // Return the ID then increment the counter
    }

     int addHalfedge(Halfedge* halfedge) {
        halfedges.push_back(halfedge);
        halfedge->id=halfedgeIDCounter;
        return halfedgeIDCounter++;  // Return the ID then increment the counter
    }

   
    // Accessors for the components
    Vertex* getVertex(int index) {
        return vertices[index];
    }

    Halfedge* getHalfedge(int index) {
        return halfedges[index];
    }

    Face* getFace(int index) {
        return faces[index];
    }

    // Accessor methods for private members
    const std::vector<Vertex*>& getVertices() const {
        return vertices;
    }

    const std::vector<Halfedge*>& getHalfedges() const {
        return halfedges;
    }

    const std::vector<Face*>& getFaces() const {
        return faces;
    }

    // Destructor to properly clean up memory
    ~HalfedgeDS() {
        for (auto v : vertices) delete v;
        for (auto he : halfedges) delete he;
        for (auto f : faces) delete f;
    }
};

#endif // HALFEDGE_DS_H
