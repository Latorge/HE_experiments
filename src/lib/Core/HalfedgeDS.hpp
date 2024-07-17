#ifndef HALFEDGE_DS_H
#define HALFEDGE_DS_H

#include <vector>
#include <set>
#include "Vertex.hpp"
#include "Halfedge.hpp"
#include "Face.hpp"



class HalfedgeDS {
private:
    std::vector<Vertex*> vertices;
    std::vector<Halfedge*> halfedges;
    std::vector<Face*> faces;

    int id;
    std::string name;

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

    HalfedgeDS(std::string _name, int _id): name(_name), id(_id) {}

    // Methods to add components to the data structure
    Vertex* addVertexDirect(glm::vec3 position);
    Vertex* addVertex(glm::vec3 position, bool checkDuplicates, float tolerance = 1e-10);

    bool removeHalfedge(Halfedge *halfedge);
    bool containsHalfedge(Halfedge *halfedge);
    bool removeFace(Face *face);
    bool removeVertex(Vertex *vertex);
    
    int addFace(Face* face);
    int addHalfedge(Halfedge* halfedge);

    // Accessor methods
    Vertex*   getVertex(int index) const { return vertices[index]; }
    Halfedge* getHalfedge(int index) const { return halfedges[index]; }
    Face*     getFace(int index) const { return faces[index]; }

    const std::vector<Vertex*>&   getVertices() const { return vertices; }
    const std::vector<Halfedge*>& getHalfedges() const { return halfedges; }
    const std::vector<Face*>&     getFaces() const { return faces; }

    // Method to get the next face ID without incrementing the counter
    int getNextFaceID() const;

    void clear();

    std::vector<std::vector<Halfedge *>> loops();
    std::vector<std::vector<Halfedge *>> loopsExp();

    // Destructor to properly clean up memory
    ~HalfedgeDS();
   
};

#endif // HALFEDGE_DS_H


