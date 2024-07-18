#include "Operations.hpp"

namespace Operations {

    /*
     * From
     *            A -------------- he -------------> B 
     *            A <------------ twin ------------- B 
     * To         
     *            A --- he ------> v ---- newhe ---> B
     *            A <-- newtwin -- v <----twin ----- B
     */

    
    Vertex* splitEdge(HalfedgeDS &structDS, Halfedge *halfedge)
    {
        return splitEdgeRatio(structDS, halfedge, 0.5);
    }

    Vertex* splitEdgeRatio(HalfedgeDS &structDS, Halfedge *halfedge, float ratio, float tolerance)
    {
        if (halfedge == nullptr || halfedge->twin == nullptr)
        {
            throw std::invalid_argument("Invalid halfedge or twin.");
        }

        Halfedge *twin = halfedge->twin;
        Vertex *A = halfedge->vertex;
        Vertex *B = twin->vertex;

        // Calculate the position of the new vertex along the edge
        glm::vec3 position = A->position + ratio * (B->position - A->position);

        // No need to split if position matches A or B
        if (glm::distance(position, A->position) < tolerance)
        {
            return A;
        }
        if (glm::distance(position, B->position) < tolerance)
        {
            return B;
        }

        Vertex* newVertex = new Vertex(position,-1);
 
        // Create the new halfedges
        Halfedge* newHalfedge = new Halfedge(newVertex);
        Halfedge* newTwin =     new Halfedge(newVertex);

        //newHalfedge->vertex = newVertex;
        //newTwin->vertex = A; // Since twin points from B to A

        newHalfedge->twin = twin;
        newTwin->twin = halfedge;
        halfedge->twin = newTwin;
        twin->twin = newHalfedge;
       

        // Update vertices' halfedge refs
        A->halfedge = halfedge;
        newVertex->halfedge = newHalfedge;
        B->halfedge = twin;

        // Update face references
        newHalfedge->face = halfedge->face;
        newTwin->face = twin->face;

        halfedge->next->prev = newHalfedge;

        // Update halfedge connections
        newHalfedge->next = halfedge->next;
        newHalfedge->prev = halfedge;


        halfedge->next = newHalfedge;
        newTwin->next = twin->next;
        newTwin->prev = twin;
        twin->next->prev = newTwin;
        twin->next = newTwin;


        structDS.addVertex(newVertex);
        structDS.addHalfedge(newHalfedge);
        structDS.addHalfedge(newTwin);

        return newVertex;
    }


    Vertex* splitEdgeByPosition(HalfedgeDS& structDS, Halfedge* halfedge, const glm::vec3& position, float tolerance) {
        if (halfedge == nullptr || halfedge->twin == nullptr) {
            throw std::invalid_argument("Invalid halfedge or twin.");
        }

        Halfedge* twin = halfedge->twin;
        Vertex* A = halfedge->vertex;
        Vertex* B = twin->vertex;

        // No need to split if position matches A or B within the given tolerance
        if (glm::distance(position, A->position) < tolerance) {
            return A;
        }
        if (glm::distance(position, B->position) < tolerance) {
            return B;
        }

        // Create the new vertex at the given position
        Vertex* newVertex = new Vertex(position, -1); // Assuming constructor takes position and ID

            // Create the new halfedges
        Halfedge* newHalfedge = new Halfedge(newVertex);
        Halfedge* newTwin =     new Halfedge(newVertex);

        //newHalfedge->vertex = newVertex;
        //newTwin->vertex = A; // Since twin points from B to A

        newHalfedge->twin = twin;
        newTwin->twin = halfedge;
        halfedge->twin = newTwin;
        twin->twin = newHalfedge;
       

        // Update vertices' halfedge refs
        A->halfedge = halfedge;
        newVertex->halfedge = newHalfedge;
        B->halfedge = twin;

        // Update face references
        newHalfedge->face = halfedge->face;
        newTwin->face = twin->face;

        halfedge->next->prev = newHalfedge;

        // Update halfedge connections
        newHalfedge->next = halfedge->next;
        newHalfedge->prev = halfedge;


        halfedge->next = newHalfedge;
        newTwin->next = twin->next;
        newTwin->prev = twin;
        twin->next->prev = newTwin;
        twin->next = newTwin;


        structDS.addVertex(newVertex);
        structDS.addHalfedge(newHalfedge);
        structDS.addHalfedge(newTwin);

        return newVertex;
    }


}
