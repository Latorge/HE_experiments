#include "front_class.hpp"
#include "iostream"
#include "algorithm"

namespace HalfedgeLib {

    void FrontClass::testCube(HalfedgeDS& halfedgeDS)
    {
       // auto protoCube=Operations::generateQuadArraysCube(1,1,1,1.0);
        auto protoCube=Operations::generateQuadArrays(15,15,1.0);
       // auto protoCube=Operations::generateQuadArraysCube(15,15,5,1.0);
/*
        for (auto& pos : protoCube.positions) {
            std::cout << "Vertex Coordinates: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
        }
        */
        auto protoCubeUniqVertex=Operations::computeUniquePositionsArray(protoCube.positions,protoCube.cells);
/*
         for (auto& pos : protoCubeUniqVertex.positions) {
            std::cout << "Vertex Coordinates: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
        }

        std::cout << "---------------------" << std::endl;
        for(auto cellArray: protoCubeUniqVertex.cells)
        {
            for(auto index:cellArray)
            {
                std::cout << index << std::endl;
            }
            std::cout << "---------" << std::endl;
        }

        std::cout << "---------------------" << std::endl;
*/
        Operations::setFromGeometry(halfedgeDS01,protoCubeUniqVertex.positions, protoCubeUniqVertex.cells);
        //Operations::setFromGeometry(halfedgeDS01,protoCube.positions, protoCube.cells);

        std::cout<<"HEDS number faces: "<<halfedgeDS01.getFaces().size()<<std::endl;
        std::cout<<"HEDS number vertex: "<<halfedgeDS01.getVertices().size()<<std::endl;
        std::cout<<"HEDS number halfEdges: "<<halfedgeDS01.getHalfedges().size()<<std::endl;

/*
        auto faceExp=halfedgeDS01.getFaces()[33];
        Operations::removeFace(halfedgeDS01,faceExp);
        auto faceExp2=halfedgeDS01.getFaces()[33];
        Operations::removeFace(halfedgeDS01,faceExp2);
        auto faceExp3=halfedgeDS01.getFaces()[33];
        Operations::removeFace(halfedgeDS01,faceExp3);
*/  
      // processRandomDeleteFace(halfedgeDS01, 1, 0.45f);
        processRandomDeleteHalfedges(halfedgeDS01, 1, 0.05f);

        std::cout<<"HEDS number faces: "<<halfedgeDS01.getFaces().size()<<std::endl;
        std::cout<<"HEDS number vertex: "<<halfedgeDS01.getVertices().size()<<std::endl;
        std::cout<<"HEDS number halfEdges: "<<halfedgeDS01.getHalfedges().size()<<std::endl;


       // auto v1=Operations::addVertex( halfedgeDS, glm::vec3(0,0,0),true);
       // auto v2=Operations::addVertex( halfedgeDS, glm::vec3(1,0,0),true);
       // auto edge1=Operations::addEdge(halfedgeDS,v1,v2);
    }

    std::vector<DrawSupport::PointInfo> FrontClass::getLinesfromHEDS(HalfedgeDS& halfedgeDS)
    {
        std::vector<DrawSupport::PointInfo> resultList;
        resultList=DrawSupport::setHalgEdgesLines2(halfedgeDS, true);
        return resultList;
    }

    DrawSupport::GeometryData FrontClass::getTrianglesfromHEDS(HalfedgeDS& halfedgeDS)
    {
        DrawSupport::GeometryData resultList;
        resultList=DrawSupport::processTriangulateHalfedgeDS(halfedgeDS);
        return resultList;
    }


    void FrontClass::processRandomDeleteHalfedges(HalfedgeDS& halfedgeDS, int divideCoeff, float rndCoeff) {
        auto heList = halfedgeDS.getHalfedges();
        // Random device and generator for shuffling
        std::random_device rd;
        std::mt19937 g(rd());
        // Shuffle the list of faces
        //std::shuffle(heList.begin(), heList.end(), g);
        // Random generator for decision making
        std::uniform_real_distribution<> dis(0.0, 1.0);
        // Process only a fraction of the list based on divideCoeff
        int limit = heList.size() / divideCoeff;
        // Vector to store faces that will be removed
        std::vector<Halfedge*> heToRemove;

        for (int i = 0; i < limit; ++i) {
            double randValue = dis(g);
            if (randValue < rndCoeff) {
                heToRemove.push_back(heList[i]);
            }
        }
        // Remove the selected faces
        for (auto he : heToRemove) {
            if(halfedgeDS01.containsHalfedge(he))
                Operations::removeEdge(halfedgeDS, he, false);
        }
    }

    
    void FrontClass::processRandomDeleteFace(HalfedgeDS& halfedgeDS, int divideCoeff, float rndCoeff) {
        auto facesList = halfedgeDS.getFaces();
        // Random device and generator for shuffling
        std::random_device rd;
        std::mt19937 g(rd());
        // Shuffle the list of faces
        std::shuffle(facesList.begin(), facesList.end(), g);
        // Random generator for decision making
        std::uniform_real_distribution<> dis(0.0, 1.0);
        // Process only a fraction of the list based on divideCoeff
        int limit = facesList.size() / divideCoeff;
        // Vector to store faces that will be removed
        std::vector<Face*> facesToRemove;

        for (int i = 0; i < limit; ++i) {
            double randValue = dis(g);
            if (randValue < rndCoeff) {
                facesToRemove.push_back(facesList[i]);
            }
        }
        // Remove the selected faces
        for (auto face : facesToRemove) {
            Operations::removeFace(halfedgeDS, face);
        }
    }
}
