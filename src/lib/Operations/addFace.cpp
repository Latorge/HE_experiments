#include "Operations.hpp"
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>

namespace Operations {

    Face* addFace(HalfedgeDS& structure, std::vector<Halfedge*>& halfedges) {
        size_t size = halfedges.size();
        if (size < 3) {
            throw std::runtime_error("At least 3 halfedges required to build a face.");
        }

        // Make some checks before changing topology
        for (size_t i = 0; i < size; i++) {
            Halfedge* curr = halfedges[i];
            Halfedge* next = halfedges[(i + 1) % size];

            if (curr->face) {
                throw std::runtime_error("Halfedge already has a face");
            }

            if (curr->twin->vertex != next->vertex) {
                throw std::runtime_error("Halfedges do not form a chain");
            }
        }

        // Add the face  
        for (size_t i = 0; i < size; i++) {
            Halfedge* curr = halfedges[i];
            Halfedge* next = halfedges[(i + 1) % size];

            if (!makeHalfedgesAdjacent(curr, next)) {
                throw std::runtime_error("Face cannot be created: mesh would be non-manifold.");
            }
        }

        Face* face = new Face(halfedges[0],-1);  // Assuming constructor takes a halfedge pointer
        for (Halfedge* halfedge : halfedges) {
            halfedge->face = face;
        }

        structure.addFace(face);  // Assuming structure has method to add faces
        return face;
    }

    bool makeHalfedgesAdjacent(Halfedge* halfIn, Halfedge* halfOut) {
        if (halfIn->next == halfOut) {
            // Adjacency is already correct
            return true;
        }

        // Find a boundary halfedge different from out->twin and in 
        /*
        Halfedge* g = nullptr; 
        auto loop = halfOut->vertex->collectFreeHalfedges();  // Assuming this returns some iterable
        for (auto he : loop) {
            if (he != halfIn) {
                g = he;
                break;
            }
        }
        */
       Halfedge* g=halfOut->vertex->findBoundaryHalfedge(halfIn, halfOut);
        if (!g) {
            return false;
        }

        Halfedge* b = halfIn->next;
        Halfedge* d = halfOut->prev;
        Halfedge* h = g->next;

        halfIn->next = halfOut;
        halfOut->prev = halfIn;

        g->next = b;
        b->prev = g;

        d->next = h;
        h->prev = d;

        return true;
    }

}
