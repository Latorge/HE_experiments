#include "Operations.hpp"

namespace Operations {

    void removeEdge(HalfedgeDS& ds, Halfedge* halfedge, bool mergeFaces) {

            /*
            *      ↖           ↙
            *        ↖       ↙
            *          ↖   ↙
            *            v2           
            *            ⇅        
            *            ⇅   
            *        he  ⇅  twin
            *            ⇅  
            *            v1
            *         ↗     ↘ 
            *       ↗         ↘
            *     ↗             ↘
            *                
            */

        Halfedge* twin = halfedge->twin;

        // Assuming HalfedgeDS has a method to remove a face and maintains a list of halfedges
        if (mergeFaces && halfedge->face && twin->face) {
            // Assuming there is a function to remove a face from the data structure
            removeFace(ds, twin->face);
            halfedge->face->halfedge = halfedge->prev;
        } else {
            // Remove both faces
            if (halfedge->face) {
                removeFace(ds, halfedge->face);
            }
            if (twin->face) {
                removeFace(ds, twin->face);
            }
        }

        // Update topology around v1
        Vertex* v1 = halfedge->vertex;
        if (twin->next == halfedge) {
            // v1 is now isolated
            v1->halfedge = nullptr;
        } else {
            v1->halfedge = twin->next;
            halfedge->prev->next = twin->next;
            twin->next->prev = halfedge->prev;
        }

        // Update topology around v2
        Vertex* v2 = twin->vertex;
        if (halfedge->next == twin) {
            // v2 is now isolated
            v2->halfedge = nullptr;
        } else {
            v2->halfedge = halfedge->next;
            halfedge->next->prev = twin->prev;
            twin->prev->next = halfedge->next;
        }

        // Remove halfedges from data structure
        ds.removeHalfedge(halfedge);
        ds.removeHalfedge(twin);
    }
/*
// Sample implementation for removing a face, assuming ds keeps a list of faces
void removeFace(HalfedgeDS& ds, Face* face) {
    ds.faces.remove(face);
    delete face; // assuming dynamic allocation
}
*/
    

}
