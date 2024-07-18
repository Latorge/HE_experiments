#include "Operations.hpp"

namespace Operations {

    Halfedge* cutFace(HalfedgeDS& structDS,
                      Face* face,
                      Vertex* v1,
                      Vertex* v2,
                      bool createNewFace) {
        if (v1 == v2) {
            throw std::invalid_argument("Vertices v1 and v2 should be different");
        }

        Halfedge* out1 = face->halfedgeFromVertex(v1);
        if (!out1 && !v1->isFree()) {
            throw std::invalid_argument("Vertex v1 does not belong to face nor is free");
        }

        Halfedge* out2 = face->halfedgeFromVertex(v2);
        if (!out2 && !v2->isFree()) {
            throw std::invalid_argument("Vertex v2 does not belong to face nor is free");
        }

        if ((out1 && out1->next->vertex == v2) || (out2 && out2->next->vertex == v1)) {
            throw std::invalid_argument("Vertices v1 and v2 are already connected");
        }

           /*
            *          From                    To
            *
            *    o → → → v1 → → → o       o → → → v1 → → → o              
            *      ↖            ↙           ↖  f  ↓↑  f' ↙       
            *        ↖    f   ↙               ↖   ↓↑   ↙    
            *          ↖    ↙                   ↖ ↓↑ ↙  
            *            v2                       v2
            * 
            *                        or
            *
            *    o → → → o → → → o        o → → → v1 → → → o              
            *      ↖   f ↓↑     ↙           ↖  f  ↓↑  f' ↙       
            *        ↖   v1   ↙               ↖   ↓↑   ↙    
            *          ↖    ↙                   ↖ ↓↑ ↙  
            *            v2                       v2
            * 
            *  --------------------------------------
            * 
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

        // Create new halfedges
        Halfedge* h1 = new Halfedge(v1);
        Halfedge* h2 = new Halfedge(v2);
        h1->face = face;
        h2->face = face;
        h1->twin = h2;
        h2->twin = h1;
        h1->next = h2;
        h2->prev = h1;

        // Adjust halfedges around v1
        if (out1) {
            Halfedge* in1 = out1->prev;
            h1->prev = in1;
            in1->next = h1;

            h2->next = out1;
            out1->prev = h2;
        } else {
            v1->halfedge = h1;
        }

        // Adjust halfedges around v2
        if (out2) {
            Halfedge* in2 = out2->prev;
            h2->prev = in2;
            in2->next = h2;

            h1->next = out2;
            out2->prev = h1;
        } else {
            v2->halfedge = h2;
        }

        // Add new halfedges to the data structure
        structDS.addHalfedge(h1);
        structDS.addHalfedge(h2);

        // Update face references and possibly create a new face
        bool isNewLoop = !isInSameLoop(h1, h2);
        if (isNewLoop)
         {
            face->halfedge = h1;
            Face* newFace = nullptr;
            if (createNewFace) {
                newFace = new Face(h2,-1);
                structDS.addFace(newFace);
            }
            updateFaceReferences(h2, newFace);
        }

        return h1;
    }

    bool isInSameLoop(Halfedge* h1, Halfedge* h2) {
        Halfedge* start = h1;
        do {
            if (h1 == h2) return true;
            h1 = h1->next;
        } while (h1 != start);
        return false;
    }

    void  updateFaceReferences(Halfedge* start, Face* newFace) {
        Halfedge* he = start;
        do {
            he->face = newFace;
            he = he->next;
        } while (he != start);
    }

    


}
