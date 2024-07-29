#ifndef HALFEDGE_DS_H
#define HALFEDGE_DS_H

#include <vector>
#include <set>
#include <map>
#include "Vertex.hpp"
#include "Halfedge.hpp"
#include "Face.hpp"
#include <random>



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

public:

    HalfedgeDS(std::string _name, int _id): name(_name), id(_id) {}

    HalfedgeDS(const HalfedgeDS& other) {
        copyFrom(other);
    }

    HalfedgeDS& operator=(const HalfedgeDS& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    // Methods to add components to the data structure
    Vertex* addVertexDirect(glm::vec3 position);
    Vertex *addVertexFromPosition(glm::vec3 position, bool checkDuplicates, float tolerance= 1e-10);

    bool containsHalfedge(Halfedge *halfedge); 
    
    bool addVertex(Vertex *vertex);
    Vertex *addVertex(glm::vec3 position);

    int addHalfedge(Halfedge *halfedge);
    Halfedge *addHalfedge(Vertex *vertex);

    int addFace(Face *face);
    Face *addFace(Halfedge *halfedge);

    bool removeVertex(Vertex *vertex);
    bool removeHalfedge(Halfedge *halfedge);
    bool removeFace(Face *face);


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

    void removeFreeVertices();

    void copyFrom(const HalfedgeDS& other);
    void mergeFrom(HalfedgeDS &other);

    Face *getRandomFace();

private:
    
};

#endif // HALFEDGE_DS_H


