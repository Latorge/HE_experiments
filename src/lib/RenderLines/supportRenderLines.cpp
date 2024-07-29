#include "RenderLines.hpp"

namespace RenderLines {
    
    std::vector<Face*> updateFrontFaces(HalfedgeDS& structD, Scene::Camera& camera) {
        std::vector<Face*> frontFaces; 
        // Loop through all faces in the mesh
        for (Face* face :structD.getFaces() ) {
            if (face->isFront(camera.position)) {
                frontFaces.push_back(face); // Add to front faces if facing towards the camera
            }
        }
        return frontFaces; 
    }

    std::unordered_set<Face*> updateFrontFacesSet(HalfedgeDS& structD, Scene::Camera& camera) {
        std::unordered_set<Face*> frontFaces; 

        // Loop through all faces in the mesh
        for (Face* face : structD.getFaces()) {
            if (face->isFront(camera.position)) {
                frontFaces.insert(face); // Add to front faces if facing towards the camera
            }
        }

        return frontFaces; 
    }


    
    

}