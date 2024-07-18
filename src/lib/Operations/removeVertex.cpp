#include "Operations.hpp"

namespace Operations {

    /*
    *         From                            To    
    * 
    * 
    *            o                              o          
    *          ↙ ⇅ ↖                          ↙   ↖        
    *        ↙   ⇅   ↖                      ↙       ↖      
    *      ↙ f1  ⇅  f4 ↖                  ↙           ↖    
    *    ↙       ⇅       ↖              ↙               ↖  
    *  o ⇄ ⇄ ⇄ ⇄ v ⇄ ⇄ ⇄ ⇄ o          o         f         o
    *    ↘       ⇅       ↗              ↘               ↗  
    *      ↘ f2  ⇅  f3 ↗                  ↘           ↗    
    *        ↘   ⇅   ↗                      ↘       ↗      
    *          ↘ ⇅ ↗                          ↘   ↗        
    *            o                              o  
    * 
    * If all halfedges starting from vertex v to delete are connected to a face, 
    * then we create a new face v. 
    * If some of the halfedges starting from v are boundaries (i.e. no face), 
    * then we can't create a new face.
    *         
    */  
    
    void removeVertex(HalfedgeDS& structDS, Vertex* vertex, bool mergeFaces ) {
        if (!vertex) return;

        // Retrieve all halfedges in a loop from the vertex
        std::vector<Halfedge*> halfedges = vertex->allHalfedgesInLoop();
        // Iterate through the halfedges and remove them
        for (Halfedge* halfedge : halfedges) {
            // Check if the halfedge is valid and has not been already processed
            if (halfedge && halfedge->twin) {
                removeEdge(structDS, halfedge, mergeFaces);
            }
        }

        // Remove the vertex from the HalfedgeDS
        structDS.removeVertex(vertex);

    }

}
