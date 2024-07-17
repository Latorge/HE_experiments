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

        Halfedge* twin_origin = halfedge->twin;
        Halfedge* halfedge_origin = halfedge;

        // Assuming HalfedgeDS has a method to remove a face and maintains a list of halfedges
        /*
        if (mergeFaces && halfedge->face && twin->face) {
            // Assuming there is a function to remove a face from the data structure
            removeFace(ds, twin->face);
            //ds.removeFace(twin->face);
            halfedge->face->halfedge = halfedge->prev;
        } else
        */
        {
            // Remove both faces
            if (halfedge->face)
            {
               // removeFace(ds, halfedge->face);
                std::vector<Halfedge*> markedHalfedges=halfedge->face->getHalfedges();
                //removeFace(ds, halfedge->face);
                ds.removeFace(halfedge->face);
                for (Halfedge* he : markedHalfedges) { 
                   he->face=nullptr;
                   if(!he->twin->face)
                   {
                      removeEdge(ds,he);
                   }
                }
               
            }

            if (twin->face) {
               // removeFace(ds, twin->face);
                std::vector<Halfedge*> markedHalfedges=twin->face->getHalfedges();
                //removeFace(ds, twin->face);
                ds.removeFace(twin->face);
                for (Halfedge* he : markedHalfedges) { 
                    he->face=nullptr;
                    if(!he->twin->face)
                    {
                        removeEdge(ds,he);
                    }
                }
                
            }
        }

        // Update topology around v1
        Vertex* v1 = halfedge->vertex;

        if(!ds.containsHalfedge(halfedge))
            return;

        if(v1->halfedge == halfedge)
        {
            if (twin->next == halfedge) {
                v1->halfedge = nullptr; // Isolated vertex
               // ds.removeVertex(v1);
            } else {
                v1->halfedge = twin->next;// Redirect to next valid halfedge  
            }
        }
        halfedge->prev->next = twin->next;
        twin->next->prev = halfedge->prev;

        if(!ds.containsHalfedge(twin))
            return;

        // Update topology around v2
        Vertex* v2 = twin->vertex;
        if(v2->halfedge == twin)
        {
            if (halfedge->next == twin) {
                v2->halfedge = nullptr; // Isolated vertex
               // ds.removeVertex(v2);
            } else {
               v2->halfedge = halfedge->next;  // Redirect to next valid halfedge
            }
        }
        twin->prev->next = halfedge->next; // Bridge the gap in the list
        halfedge->next->prev = twin->prev;

        // Remove halfedges from data structure
        ds.removeHalfedge(halfedge_origin);
        ds.removeHalfedge(twin_origin);
    }
/*
// Sample implementation for removing a face, assuming ds keeps a list of faces
void removeFace(HalfedgeDS& ds, Face* face) {
    ds.faces.remove(face);
    delete face; // assuming dynamic allocation
}
*/
/* samplpe code
 void removeEdge(HalfedgeDS& ds, Halfedge* halfedge, bool mergeFaces) {
*/
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
/*
        Halfedge* twin = halfedge->twin;

        // Assuming HalfedgeDS has a method to remove a face and maintains a list of halfedges
        if (mergeFaces && halfedge->face && twin->face) {
            // Assuming there is a function to remove a face from the data structure
            removeFace(ds, twin->face);
            //ds.removeFace(twin->face);
            halfedge->face->halfedge = halfedge->prev;
        } else {
            // Remove both faces
            if (halfedge->face) {
                //removeFace(ds, halfedge->face);
                ds.removeFace(halfedge->face);
            }
            if (twin->face) {
                //removeFace(ds, twin->face);
                ds.removeFace(twin->face);
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
*/

}
