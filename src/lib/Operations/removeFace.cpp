#include "Operations.hpp"
#include <algorithm>

namespace Operations {

    bool removeFace(HalfedgeDS& ds, Face* face) {
        
        std::vector<Halfedge*> markedHalfedges=face->getHalfedges();
        for (Halfedge* he : markedHalfedges) { 
            he->face=nullptr;
            if (he->twin && he->twin->face == nullptr) {
               removeEdge(ds, he);
            }
        }
        ds.removeFace(face);
        return true;
    }

/*proto code
    bool removeFace(HalfedgeDS& ds, Face* face) {
        std::vector<Halfedge*> markedHalfedges;

        // Assuming face->halfedge points to one halfedge on the face
        Halfedge* start = face->halfedge;
        Halfedge* he = start;

        do {
            he->face = nullptr;  // Remove face reference from halfedges
            if (he->twin && he->twin->face == nullptr) {
                markedHalfedges.push_back(he);
            }
            he = he->next;  // Move to the next halfedge in the loop
        } while (he != start);

        // Remove the marked halfedges
        for (Halfedge* halfedge : markedHalfedges) {
            removeEdge(ds, halfedge);
            //ds.removeHalfedge(halfedge);
            if (halfedge->twin) {
                removeEdge(ds, halfedge->twin);
                //ds.removeHalfedge( halfedge->twin);
            }
        }

        ds.removeFace(face);

        return true;
    }
 */


}
