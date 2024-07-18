#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <vector>
#include <set>
#include <glm/glm.hpp>
#include "Struct.hpp"

#include "Halfedge.hpp"

class Vertex {
public:
    glm::vec3 position;
    Halfedge* halfedge  =   nullptr;
    bool needUpdate     =   false;
    int subGroup        = -1;
    int id;

    Vertex(int& _id);
    Vertex(float x, float y, float z, int& _id);
    Vertex(glm::vec3 vec, int _id);

    void changeComponent(float x, float y, float z) ;
    void change(const glm::vec3& vector) ;
    void move(const glm::vec3& vector) ;

    std::vector<Halfedge *> freeHalfedgesInLoop(Halfedge *start);

    Halfedge* freeHalfedgesInLoopNext(Halfedge *start);

    Halfedge* findBoundaryHalfedge(Halfedge *halfIn, Halfedge *halfOut);

    std::vector<Halfedge *> collectBoundaryHalfedges();
    
    bool isFree() ;
    bool isIsolated();

    std::vector<Face*> commonFacesWithVertex(Vertex* other) ;
    bool matchesPosition(const glm::vec3& pos, float tolerance ) ;

    Halfedge* getHalfedgeToVertex(Vertex* other);
    bool isConnectedToVertex(Vertex* other);
     glm::vec3 calculateVertexNormal();
    //std::vector<Halfedge*> loopCW() ;


    class CWIterator {
    private:
        Halfedge* start;
        Halfedge* current;
        bool firstPass;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Halfedge;
        using difference_type = std::ptrdiff_t;
        using pointer = Halfedge*;
        using reference = Halfedge&;

        CWIterator(Halfedge* start, bool isFirst = true) : start(start), current(start), firstPass(isFirst) {}

        reference operator*() const { return *current; }
        pointer operator->() { return current; }

        // Prefix increment
        CWIterator& operator++();

        // Postfix increment
        CWIterator operator++(int);

        bool hasNext() const;

        Halfedge* next();

        void reset();


        friend bool operator==(const CWIterator& a, const CWIterator& b) {
            return a.current == b.current;
        }
        friend bool operator!=(const CWIterator& a, const CWIterator& b) {
            return a.current != b.current;
        }
    };

    CWIterator beginCW(Halfedge* start = nullptr) { return CWIterator(start ? start : halfedge); }
    CWIterator endCW() { return CWIterator(nullptr, false); }

    class CCWIterator {
    private:
        Halfedge* start;
        Halfedge* current;
        bool firstPass;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Halfedge;
        using difference_type = std::ptrdiff_t;
        using pointer = Halfedge*;
        using reference = Halfedge&;

        CCWIterator(Halfedge* start, bool isFirst = true) : start(start), current(start), firstPass(isFirst) {}

        reference operator*() const { return *current; }
        pointer operator->() { return current; }

        // Prefix decrement
        CCWIterator& operator--();

        // Postfix decrement
        CCWIterator operator--(int);

        friend bool operator==(const CCWIterator& a, const CCWIterator& b) {
            return a.current == b.current;
        }
        friend bool operator!=(const CCWIterator& a, const CCWIterator& b) {
            return a.current != b.current;
        }
    };

    CCWIterator beginCCW(Halfedge* start = nullptr) { return CCWIterator(start ? start : halfedge); }
    CCWIterator endCCW() { return CCWIterator(nullptr, false); }

private:
   
    float calculateAngleWeight(Face *face, const glm::vec3 &faceNormal, size_t numCommonVertices);
};


#endif // VERTEX_HPP

