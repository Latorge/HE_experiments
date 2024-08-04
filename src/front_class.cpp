#include "front_class.hpp"
#include "iostream"
#include "algorithm"

namespace HalfedgeLib {

    void FrontClass::initSurfRender() {

        //HalfedgeDS triangleHalfedgeDS=halfedgeDS;
        Modificators::triangleSubDivideStruct(halfedgeDS01);
        surfRender=new RenderLines::SurfRender(&halfedgeDS01,3);
    }

    std::vector<DrawSupport::PointInfo> FrontClass::step(){
        std::vector<DrawSupport::PointInfo> resultList;

        surfRender->update(0.01f);
        resultList=surfRender->collectTrailSegments();
        
        std::vector<DrawSupport::PointInfo> resultList2;
        resultList2=surfRender->drawProjectedAgentsAndFaces(0.1);

        // Append resultList2 to resultList
        resultList.insert(resultList.end(), resultList2.begin(), resultList2.end());

        return resultList;
    }


    std::vector<DrawSupport::PointInfo> FrontClass::getLinesfromHEDS(HalfedgeDS& halfedgeDS, bool onlyBoundaryLines, bool drawArrows){
        std::vector<DrawSupport::PointInfo> resultList;
        resultList=DrawSupport::setHalfEdgesLines2(halfedgeDS, onlyBoundaryLines, drawArrows);
        return resultList;
    }

    std::vector<DrawSupport::PointInfo> FrontClass::getSilhouetteLinesfromHEDS(HalfedgeDS& halfedgeDS, Scene::Camera& camera) {
        std::vector<DrawSupport::PointInfo> resultList;

        DrawSupport::Color colorTemp1;
        colorTemp1.setRGB(0.75f,0.75f,0.5f,1.0f);

        HalfedgeDS triangleHalfedgeDS=halfedgeDS;
        Modificators::triangleSubDivideStruct(triangleHalfedgeDS);
        std::unordered_set<Face*> frontFaces = RenderLines::updateFrontFacesSet(triangleHalfedgeDS, camera);

        resultList=RenderLines::renderSilhouetteLines(triangleHalfedgeDS, frontFaces, colorTemp1, camera);
        DrawSupport::Color colorTemp;
        colorTemp.setRGB(0.5f,0.75f,0.75f,1.0f);
        
        auto resultList2=RenderLines::renderSilhouetteLinesWithAngle(triangleHalfedgeDS,frontFaces,colorTemp, 35);

        // Append resultList2 to resultList
        resultList.insert(resultList.end(), resultList2.begin(), resultList2.end());

        return resultList;
    }

    DrawSupport::GeometryData FrontClass::getTrianglesfromHEDS(HalfedgeDS& halfedgeDS){
        DrawSupport::GeometryData resultList;
        resultList=DrawSupport::processTriangulateHalfedgeDS(halfedgeDS);
        return resultList;
    }

    void FrontClass::triangleSubDivideHEDS(HalfedgeDS& halfedgeDS){
        Modificators::triangleSubDivideStruct(halfedgeDS);
    }

    DrawSupport::GeometryData FrontClass::getDebugLines(){
        return debugLines;
    }

    void FrontClass::quadSubDivideHEDS(HalfedgeDS& halfedgeDS){
        Modificators::quadSubDivideStruct(halfedgeDS);
        //intersectionAABB(halfedgeDS);
    }


    void FrontClass::catmullClarkHEDS(HalfedgeDS& halfedgeDS){
         Modificators::catmullClarkExp(halfedgeDS01);
    }

    void FrontClass::processFaceRecursiveOppositeEdges(HalfedgeDS& halfedgeDS,float rndCoeff, int recursiveDepth){
        processRandomFace(halfedgeDS01, 1, rndCoeff, Operations::processFaceRecursiveOppositeEdges, recursiveDepth);
        halfedgeDS01.removeFreeVertices();
    }


    //processRandomDeleteFace(halfedgeDS01, 1, 0.45f);
    void FrontClass::randomDeleteFaces(HalfedgeDS& halfedgeDS, float rndCoeff){
       processRandomDeleteFace(halfedgeDS, 1, rndCoeff);
       halfedgeDS01.removeFreeVertices();
    }

    void FrontClass::randomExtrudeFaces(HalfedgeDS& halfedgeDS, float rndCoeff, float distExtrude, float amountExtrude){
       processRandomFaceT(halfedgeDS, 1, rndCoeff, Modificators::extrudeFace2, distExtrude, amountExtrude);
       //halfedgeDS01.removeFreeVertices();
    }

    void FrontClass::clearSelectedFaces(){
       selectedFaces.clear();
    }

    void FrontClass::selectedExtrudeFaces(HalfedgeDS& halfedgeDS,  std::vector<Face*> selectedFaces, float distExtrude, float amountExtrude){
       processSelectedFaceT(halfedgeDS, selectedFaces, Modificators::extrudeFace2, distExtrude, amountExtrude);
       //halfedgeDS01.removeFreeVertices();
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

    void FrontClass::processRandomFace(HalfedgeDS& halfedgeDS, int divideCoeff, float rndCoeff, 
                           std::function<void(HalfedgeDS&, Face*, int)> func, int iterations) {
        auto facesList = halfedgeDS.getFaces();
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(facesList.begin(), facesList.end(), g);
        std::uniform_real_distribution<> dis(0.0, 1.0);
        int limit = facesList.size() / divideCoeff;
        std::vector<Face*> facesToProcess;

        for (int i = 0; i < limit; ++i) {
            if (dis(g) < rndCoeff) {
                facesToProcess.push_back(facesList[i]);
            }
        }

        for (auto face : facesToProcess) {
            func(halfedgeDS, face, iterations);
        }
    }

    std::vector<Face*> FrontClass::selectRandomFaces(HalfedgeDS& halfedgeDS, int divideCoeff, float rndCoeff) {
        auto facesList = halfedgeDS.getFaces();
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(facesList.begin(), facesList.end(), g);
        std::uniform_real_distribution<> dis(0.0, 1.0);
        int limit = facesList.size() / divideCoeff;
        std::vector<Face*> facesToProcess;

        for (int i = 0; i < limit; ++i) {
            if (dis(g) < rndCoeff) {
                facesToProcess.push_back(facesList[i]);
            }
        }
        return facesToProcess;
    }
    //processRandomFace(halfedgeDS01, 10, 0.5, Operations::processFaceRecursiveOppositeEdges, 11);

    template<typename Func, typename... Args>
    void FrontClass::processRandomFaceT(HalfedgeDS& halfedgeDS, int divideCoeff, float rndCoeff, Func func, Args&&... args) {
        auto facesList = halfedgeDS.getFaces();
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(facesList.begin(), facesList.end(), g);
        std::uniform_real_distribution<> dis(0.0, 1.0);
        int limit = facesList.size() / divideCoeff;
        std::vector<Face*> facesToProcess;

        for (int i = 0; i < limit; ++i) {
            if (dis(g) < rndCoeff) {
                facesToProcess.push_back(facesList[i]);
            }
        }

        for (auto face : facesToProcess) {
            std::invoke(func, halfedgeDS, face, std::forward<Args>(args)...);
        }
    }

    template<typename Func, typename... Args>
    void FrontClass::processSelectedFaceT(HalfedgeDS& halfedgeDS, std::vector<Face*> selectedFaces, Func func, Args&&... args) {
        for (auto face : selectedFaces) {
            std::invoke(func, halfedgeDS, face, std::forward<Args>(args)...);
        }
    }

    //processRandomFace(halfedgeDS01, 10, 0.5, Operations::processFaceRecursiveOppositeEdges, 11);  // 11 as the iteration count
}
