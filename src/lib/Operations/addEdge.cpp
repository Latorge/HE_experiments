#include <stdexcept> // For std::invalid_argument
#include "Operations.hpp"

namespace Operations {

    Halfedge* addEdge(
           HalfedgeDS& structDS,
           Vertex* v1,
           Vertex* v2,
           bool allowParallels) {

        if (v1 == v2) {
            throw std::invalid_argument("Vertices v1 and v2 should be different");
        }

        if (!allowParallels) {
            // Check if v1 and v2 are already connected
            Halfedge* currentHalfEdge = v1->getHalfedgeToVertex(v2);
            if (currentHalfEdge) {
                return currentHalfEdge;
            }
        }

        if (!v1->isFree() || !v2->isFree()) {
            throw std::invalid_argument("Vertices v1 and v2 are not free");
        }

        // Create new halfedges
        Halfedge* h1 = new Halfedge(v1);
        Halfedge* h2 = new Halfedge(v2);
        h1->twin = h2;
        h1->next = h2; // These settings assume isolated vertices
        h1->prev = h2;
        h2->twin = h1;
        h2->next = h1;
        h2->prev = h1;

           /*
            *        ↖       ↙
            *   out2   ↖   ↙   in2
            *            v2           
            *            ⇅        
            *            ⇅   
            *        h1  ⇅  h2     
            *            ⇅  
            *            ⇅  
            *            v1
            *    in1  ↗     ↘  out1
            *       ↗         ↘
            *            
            */

        // Update refs around v1 if not isolated
        Halfedge* in1 = v1->freeHalfedgesInLoopIter().next(); // Custom function to find a free halfedge
        if (in1) {
            Halfedge* out1 = in1->next->next;
            h1->prev = in1;
            in1->next = h1;

            h2->next = out1;
            out1->prev = h2;
        } else {
            v1->halfedge = h1; // Update v1's halfedge if it was isolated
        }

        // Update refs around v2 if not isolated
        Halfedge* in2 = v2->freeHalfedgesInLoopIter().next();
        if (in2) {
            Halfedge* out2 = in2->next;
            h2->prev = in2;
            in2->next = h2;

            h1->next = out2;
            out2->prev = h1;
        } else {
            v2->halfedge = h2; // Update v2's halfedge if it was isolated
        }

        structDS.addHalfedge(h1);
        structDS.addHalfedge(h2);

        return h1;
    }

}
