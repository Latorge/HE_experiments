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
            Halfedge* currentHalfEdge = v1->getHalfedgeToVertex(v2);
            if (currentHalfEdge) {
                return currentHalfEdge;
            }
            // Handle the error case where no connecting half-edge is found
            throw std::runtime_error("Vertices v1 and v2 are not free or not connected by a halfedge.");
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

        std::cout<<"------"<<std::endl;

        //HalfedgeGenerator gen1(v1);
        //auto in1 = gen1.next();

        Halfedge* in1 = v1->freeHalfedgesInLoopNext(nullptr);// Custom function to find a free halfedge
        if (in1) {
            //auto in1he=*in1;
            auto in1he=in1;
            std::cout<<in1he->getId()<<std::endl;
            Halfedge* out1 = in1he->next;
            h1->prev = in1he;
            in1he->next = h1;

            h2->next = out1;
            out1->prev = h2;
        } else {
            v1->halfedge = h1; // Update v1's halfedge if it was isolated
        }

        // Update refs around v2 if not isolated
        //HalfedgeGenerator gen2(v2);
        //auto in2 = gen2.next();

        Halfedge* in2 = v2->freeHalfedgesInLoopNext(nullptr);//->freeHalfedgesInLoopIter().next();
        if (in2) {
            //auto in2he=*in2;
            auto in2he=in2;
            std::cout<<in2he->getId()<<std::endl;
            Halfedge* out2 = in2he->next;
            h2->prev = in2he;
            in2he->next = h2;

            h1->next = out2;
            out2->prev = h1;
        } else {
            v2->halfedge = h2; // Update v2's halfedge if it was isolated
        }
        //std::cout<<h1->getId()<<std::endl;
        //std::cout<<h2->getId()<<std::endl;
        std::cout<<"------"<<std::endl;
        structDS.addHalfedge(h1);
        structDS.addHalfedge(h2);

        return h1;
    }


/*
Vertex* v; // Assume this is properly initialized
HalfedgeGenerator gen(v);

auto he = gen.next();
while (he) {
    Halfedge* freeHalfedge = *he;
    // Process freeHalfedge

    he = gen.next(); // Move to the next free halfedge
}
*/
    HalfedgeGenerator::HalfedgeGenerator(Vertex* v, Halfedge* s)
        : vertex(v), start(s ? s : v->halfedge), current(s ? s : v->halfedge), firstCall(true) {}

    std::optional<Halfedge*> HalfedgeGenerator::next() {
        if (!current || !start) return std::nullopt;  // Handle cases where there is no starting halfedge

        if (!firstCall) {
            current = current->twin->next;
        }

        if (current == start && !firstCall) {
            return std::nullopt; // Completed a full loop
        }

        firstCall = false;

        if (current->twin && current->twin->isFree()) {
            return {current->twin};
        }

        return next(); // Continue to the next valid halfedge if the current one isn't free
    }

    void HalfedgeGenerator::reset() {
        current = start;
        firstCall = true;
    }

}
