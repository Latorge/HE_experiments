#include "front_class.hpp"
#include "iostream"
#include "algorithm"

namespace HalfedgeLib {

    void FrontClass::testCube(HalfedgeDS& halfedgeDS)
    {
        //auto protoCube=Operations::generateQuadArraysCube(1,1,1,1.0);
        //auto protoCube=Operations::generateQuadArrays(1,1,1.0);
        //auto protoCube=Operations::generateQuadArrays(3,3,1.0);

       // auto protoCube=Operations::generateQuadArrays(11,11,1.0);
       // auto protoCube=Operations::generateQuadArraysCube(5,5,5,1.0);

        std::vector<glm::vec3> sourcePoints;
        int N = 30; // Number of points
        float radius = 35.0f; // Radius of the half-circle
        fillHalfCirclePoints(sourcePoints, N, radius);
        auto protoCube=Operations::generateTube(sourcePoints,7,0,0);
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
        //Operations::setFromGeometry(halfedgeDS01,protoCubeUniqVertex.positions, protoCubeUniqVertex.cells);
        //Operations::setFromGeometry(halfedgeDS01,protoCube.positions, protoCube.cells);

    

        std::cout<<"HEDS number faces: "<<halfedgeDS01.getFaces().size()<<std::endl;
        std::cout<<"HEDS number vertex: "<<halfedgeDS01.getVertices().size()<<std::endl;
        std::cout<<"HEDS number halfEdges: "<<halfedgeDS01.getHalfedges().size()<<std::endl;

        {
            HalfedgeDS temphalfedgeDS("temp",2);
            std::vector<glm::vec3> sourcePoints;
            int N = 20; // Number of points
            float radius = 12.0f; // Radius of the half-circle
            fillSpiralPointsNAU(sourcePoints, N, 1.5,12.5, 5, 2.75);
            auto protoCube=Operations::generateTubeNL(sourcePoints,0.25,9,0,0);

            //auto protoCube=Operations::generateQuadArraysCube(3,3,3,1.0);
            auto protoCubeUniqVertex=Operations::computeUniquePositionsArray(protoCube.positions,protoCube.cells);

            //Operations::setFromGeometry(halfedgeDS01,protoCubeUniqVertex.positions, protoCubeUniqVertex.cells);
            Operations::setFromGeometry(temphalfedgeDS,protoCube.positions, protoCube.cells);

            halfedgeDS01.mergeFrom(temphalfedgeDS);
        }

/*
        auto faceExp=halfedgeDS01.getFaces()[33];
        Operations::removeFace(halfedgeDS01,faceExp);
        auto faceExp2=halfedgeDS01.getFaces()[33];
        Operations::removeFace(halfedgeDS01,faceExp2);
        auto faceExp3=halfedgeDS01.getFaces()[33];
        Operations::removeFace(halfedgeDS01,faceExp3);
*/  
       //processRandomDeleteFace(halfedgeDS01, 1, 0.45f);
       //halfedgeDS01.removeFreeVertices();
       //  processRandomDeleteHalfedges(halfedgeDS01, 1, 0.075f);
/*
        auto faceTemp= halfedgeDS01.getFace(0);
        auto vertexList=faceTemp->getVertices();
        //Operations::cutFace(halfedgeDS01, faceTemp,vertexList[0], vertexList[2], true);

        auto newVertex= Operations::splitEdgeRatio(halfedgeDS01, faceTemp->getHalfedges()[2], 0.5);
        auto newVertex2=Operations::splitEdgeRatio(halfedgeDS01, faceTemp->getHalfedges()[0], 0.5);

        Operations::cutFace(halfedgeDS01,faceTemp,newVertex,newVertex2, true);
*/

        //auto faceTemp= halfedgeDS01.getFace(0);
        //Modificators::extrudeFace(halfedgeDS01,faceTemp, 1.1f,0.5f);
        
        auto vertexTemp= halfedgeDS01.getVertex(8);
        std::vector<Halfedge*> neighbors = vertexTemp->allHalfedgesInLoop();
        std::cout<<"--------------------"<<std::endl;
        std::vector<Halfedge*> neighbors3 = vertexTemp->allHalfedgesInLoopCCW();
        std::cout<<"--------------------"<<std::endl;
        std::vector<Halfedge*> neighbors2 = vertexTemp->allOutgoingHalfedges();
        std::cout<<"--------------------"<<std::endl;

        std::cout<<"number neibors: "<<neighbors.size()<<std::endl;
        std::cout<<"number neibors: "<<neighbors3.size()<<std::endl;
        std::cout<<"number neibors outgoing: "<<neighbors2.size()<<std::endl;
/*
        {   
            HalfedgeDS halfedgeDS02=halfedgeDS01;
            //auto faceTemp= halfedgeDS01.getFace(0);
            auto vertexTemp= halfedgeDS02.getVertex(8);
            std::vector<Halfedge*> neighbors = vertexTemp->allHalfedgesInLoop();
            std::cout<<"--------------------"<<std::endl;
            std::vector<Halfedge*> neighbors3 = vertexTemp->allHalfedgesInLoopCCW();
            std::cout<<"--------------------"<<std::endl;
            std::vector<Halfedge*> neighbors2 = vertexTemp->allOutgoingHalfedges();
            std::cout<<"--------------------"<<std::endl;

            std::cout<<"number neibors: "<<neighbors.size()<<std::endl;
            std::cout<<"number neibors: "<<neighbors3.size()<<std::endl;
            std::cout<<"number neibors outgoing: "<<neighbors2.size()<<std::endl;


            std::cout<<"HEDS2 number faces: "<<halfedgeDS02.getFaces().size()<<std::endl;
            std::cout<<"HEDS2 number vertex: "<<halfedgeDS02.getVertices().size()<<std::endl;
            std::cout<<"HEDS2 number halfEdges: "<<halfedgeDS02.getHalfedges().size()<<std::endl;
        }
*/
       // auto commonFaces=vertexTemp->commonFacesWithVertex();
      //  std::cout<<"numbercommon faces: "<<commonFaces.size()<<std::endl;


       
       // Modificators::catmullClarkExp(halfedgeDS01);
       // Modificators::catmullClarkExp(halfedgeDS01);
       //   Modificators::catmullClarkExp(halfedgeDS01);

       // Modificators::quadSubDivideStruct(halfedgeDS01);


        //Operations::removeVertex(halfedgeDS01, vertexTemp);

        //Operations::processFaceRecursive(halfedgeDS01, faceTemp, 5);
        //Operations::processFaceRecursiveOppositeEdges(halfedgeDS01, faceTemp, 11);
/*
        processRandomFace(halfedgeDS01, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 1);
        processRandomFace(halfedgeDS01, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 1);
        processRandomFace(halfedgeDS01, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 2);
        processRandomFace(halfedgeDS01, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 2);
        processRandomFace(halfedgeDS01, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 2);

        processRandomDeleteFace(halfedgeDS01, 1, 0.45f);
        processRandomDeleteHalfedges(halfedgeDS01, 1, 0.015f);
*/
        std::cout<<"HEDS number faces: "<<halfedgeDS01.getFaces().size()<<std::endl;
        std::cout<<"HEDS number vertex: "<<halfedgeDS01.getVertices().size()<<std::endl;
        std::cout<<"HEDS number halfEdges: "<<halfedgeDS01.getHalfedges().size()<<std::endl;


       // auto v1=Operations::addVertex( halfedgeDS, glm::vec3(0,0,0),true);
       // auto v2=Operations::addVertex( halfedgeDS, glm::vec3(1,0,0),true);
       // auto edge1=Operations::addEdge(halfedgeDS,v1,v2);
    }

    void FrontClass::intersectionAABB(HalfedgeDS& halfedgeDS){
        DrawSupport::GeometryData resultList;
        resultList=DrawSupport::processTriangulateHalfedgeDS(halfedgeDS);

        intersections::TriangleMeshAABB mesh;
        // Suppose this is your buffer of glm::vec3 points:
        std::vector<glm::vec3> points = resultList.vertices;
        mesh.addTrianglesFromBuffer(points);

        // Example query
        intersections::Kernel::Ray_3 ray_query(intersections::Point(1.0, 0.0, 0.0), intersections::Point(0.0, 1.0, 0.0));
        std::cout << mesh.countIntersections(ray_query) << " intersection(s) with ray query" << std::endl;

        intersections::Point point_query(2.0, 2.0, 2.0);
        std::cout << "Closest point is: " << mesh.closestPoint(point_query) << std::endl;
        std::cout << "Squared distance: " << mesh.squaredDistance(point_query) << std::endl;
        
    }

    void FrontClass::fillHalfCirclePoints(std::vector<glm::vec3>& sourcePoints, int N, float radius) {
        if (N < 2) {
            std::cerr << "Error: N must be at least 2 to form a half-circle." << std::endl;
            return;
        }

        float angleIncrement = 2.0f*glm::pi<float>() / (N - 1); // Divide the half-circle into (N-1) segments

        for (int i = 0; i < N; ++i) {
            float theta = i * angleIncrement; // Current angle
            float x = radius * cos(theta);    // Calculate x coordinate
            float z = radius * sin(theta);    // Calculate z coordinate
            sourcePoints.push_back(glm::vec3(x, 0, z)); // y is zero since it's on the xz-plane
        }
    }

    void FrontClass::fillSpiralPoints(std::vector<glm::vec3>& sourcePoints, int N, float initialRadius, float finalRadius, int numberOfLoops) {
        if (N < 2) {
            std::cerr << "Error: N must be at least 2 to form a meaningful spiral." << std::endl;
            return;
        }
        if (numberOfLoops < 1) {
            std::cerr << "Error: numberOfLoops must be at least 1." << std::endl;
            return;
        }

        // Total number of points is N points per loop times the number of loops
        float totalHeight = 1.0f; // Total height of the spiral from start to end
        float heightIncrement = totalHeight / (numberOfLoops * (N - 1)); // Height increment per point

        // Calculate the total angle and the radius increment
        float totalAngle = 2.0f * glm::pi<float>() * numberOfLoops; // Total angle covered by the spiral
        float radiusIncrement = (finalRadius - initialRadius) / (numberOfLoops * (N - 1)); // Change in radius per point

        for (int loop = 0; loop < numberOfLoops; ++loop) {
            for (int i = 0; i < N; ++i) {
                float fraction = (loop * N + i) / float(numberOfLoops * (N - 1)); // Fraction of the way through the loops
                float theta = fraction * totalAngle; // Current angle
                float radius = initialRadius + fraction * (finalRadius - initialRadius); // Current radius
                float x = radius * cos(theta);    // Calculate x coordinate
                float z = radius * sin(theta);    // Calculate z coordinate
                float y = fraction * totalHeight; // Current height based on the fraction of the total loops

                sourcePoints.push_back(glm::vec3(x, y, z));
            }
        }
    }

    void FrontClass::fillSpiralPointsNAU(std::vector<glm::vec3>& sourcePoints, int N, float initialRadius, float finalRadius, int numberOfLoops, float radiusCoeff) {
        if (N < 2) {
            std::cerr << "Error: N must be at least 2 to form a meaningful spiral." << std::endl;
            return;
        }
        if (numberOfLoops < 1) {
            std::cerr << "Error: numberOfLoops must be at least 1." << std::endl;
            return;
        }

        // Total number of points is N points per loop times the number of loops
        float totalHeight = 1.0f; // Total height of the spiral from start to end
        float heightIncrement = totalHeight / (numberOfLoops * (N - 1)); // Height increment per point

        // Calculate the total angle covered by the spiral
        float totalAngle = 2.0f * glm::pi<float>() * numberOfLoops;

        for (int loop = 0; loop < numberOfLoops; ++loop) {
            for (int i = 0; i < N; ++i) {
                float fraction = (loop * N + i) / float(numberOfLoops * N - 1); // Fraction of the way through the loops
                float theta = fraction * totalAngle; // Current angle

                // Calculate the current radius starting from near zero to the finalRadius using exponential growth
                float radius = (exp(radiusCoeff * fraction) - 1) * finalRadius / (exp(radiusCoeff) - 1);

                float x = radius * cos(theta);    // Calculate x coordinate
                float z = radius * sin(theta);    // Calculate z coordinate
                float y = fraction * totalHeight; // Current height based on the fraction of the total loops

                sourcePoints.push_back(glm::vec3(x, y, z));
            }
        }
    }




    std::vector<DrawSupport::PointInfo> FrontClass::getLinesfromHEDS(HalfedgeDS& halfedgeDS, bool onlyBoundaryLines, bool drawArrows){
        std::vector<DrawSupport::PointInfo> resultList;
        resultList=DrawSupport::setHalfEdgesLines2(halfedgeDS, onlyBoundaryLines, drawArrows);
        return resultList;
    }

    DrawSupport::GeometryData FrontClass::getTrianglesfromHEDS(HalfedgeDS& halfedgeDS){
        DrawSupport::GeometryData resultList;
        resultList=DrawSupport::processTriangulateHalfedgeDS(halfedgeDS);
        return resultList;
    }

    DrawSupport::GeometryData FrontClass::getDebugLines(){
        return debugLines;
    }

    void FrontClass::quadSubDivideHEDS(HalfedgeDS& halfedgeDS){
        //Modificators::quadSubDivideStruct(halfedgeDS);
        intersectionAABB(halfedgeDS);
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
