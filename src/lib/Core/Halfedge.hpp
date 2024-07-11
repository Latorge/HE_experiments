#ifndef HALFEDGE_HPP
#define HALFEDGE_HPP

#include <iostream>
#include <functional>
#include <optional>
#include <glm/glm.hpp>
#include "Struct.hpp"
#include "Geometry.hpp"

class Halfedge {
public:
    bool      visited = false;
    Vertex*    vertex = nullptr;;
    Face*      face   = nullptr;
    Halfedge*  twin   = nullptr;
    Halfedge*  prev   = nullptr;
    Halfedge*  next   = nullptr;
    bool      copied = false;
    int id;

    Halfedge(Vertex* vertex, int _id=-1);

    std::string getId() const ;

    std::string getIdReverse() const ;

    bool containsPoint(const glm::vec3& point, float tolerance) ;

    bool isFree() const ;

    bool isBoundary() const ;

    bool isConcave() const ;

    void forEachHalfedgeCW(const std::function<void(Halfedge*)>& action);

    void forEachHalfedgeCCW(const std::function<void(Halfedge*)>& action) ;

    class Iterator {
    private:
        Halfedge* ptr;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Halfedge;
        using difference_type = std::ptrdiff_t;
        using pointer = Halfedge*;
        using reference = Halfedge&;

        explicit Iterator(Halfedge* p = nullptr) : ptr(p) {}

        reference operator*() const { return *ptr; }
        pointer operator->() { return ptr; }

        // Prefix increment
        Iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        // Postfix increment
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; }
        friend bool operator!=(const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; }
    };

    Iterator begin() { return Iterator(this); }
    Iterator end() { return Iterator(); }

private:
    glm::vec3 closestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p) ;
};



#endif // HALFEDGE_HPP
