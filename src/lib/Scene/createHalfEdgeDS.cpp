#include "Scene.hpp"


namespace Scene {

    void Scene::testCube(HalfedgeDS& halfedgeDS)
    {
        auto protoCube=Operations::generateQuadArraysCube(1,1,1,1.0);
        //auto protoCube=Operations::generateQuadArrays(1,1,1.0);
        //auto protoCube=Operations::generateQuadArrays(3,3,1.0);

       // auto protoCube=Operations::generateQuadArrays(11,11,1.0);
       // auto protoCube=Operations::generateQuadArraysCube(5,5,5,1.0);

       // std::vector<glm::vec3> sourcePoints;
       // int N = 30; // Number of points
       // float radius = 35.0f; // Radius of the half-circle
       // fillHalfCirclePoints(sourcePoints, N, radius);
       // auto protoCube=Operations::generateTube(sourcePoints,7,0,0);
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
        Operations::setFromGeometry(halfedgeDS,protoCubeUniqVertex.positions, protoCubeUniqVertex.cells);
        //Operations::setFromGeometry(halfedgeDS,protoCube.positions, protoCube.cells);


        std::cout<<"HEDS number faces: "<<halfedgeDS.getFaces().size()<<std::endl;
        std::cout<<"HEDS number vertex: "<<halfedgeDS.getVertices().size()<<std::endl;
        std::cout<<"HEDS number halfEdges: "<<halfedgeDS.getHalfedges().size()<<std::endl;

/*
        auto faceExp=halfedgeDS.getFaces()[33];
        Operations::removeFace(halfedgeDS,faceExp);
        auto faceExp2=halfedgeDS.getFaces()[33];
        Operations::removeFace(halfedgeDS,faceExp2);
        auto faceExp3=halfedgeDS.getFaces()[33];
        Operations::removeFace(halfedgeDS,faceExp3);
*/  
       //processRandomDeleteFace(halfedgeDS, 1, 0.45f);
       //halfedgeDS.removeFreeVertices();
       //  processRandomDeleteHalfedges(halfedgeDS, 1, 0.075f);
/*
        auto faceTemp= halfedgeDS.getFace(0);
        auto vertexList=faceTemp->getVertices();
        //Operations::cutFace(halfedgeDS, faceTemp,vertexList[0], vertexList[2], true);

        auto newVertex= Operations::splitEdgeRatio(halfedgeDS, faceTemp->getHalfedges()[2], 0.5);
        auto newVertex2=Operations::splitEdgeRatio(halfedgeDS, faceTemp->getHalfedges()[0], 0.5);

        Operations::cutFace(halfedgeDS,faceTemp,newVertex,newVertex2, true);
*/

        //auto faceTemp= halfedgeDS.getFace(0);
        //Modificators::extrudeFace(halfedgeDS,faceTemp, 1.1f,0.5f);
        
        /*
        auto vertexTemp= halfedgeDS.getVertex(8);
        std::vector<Halfedge*> neighbors = vertexTemp->allHalfedgesInLoop();
        std::cout<<"--------------------"<<std::endl;
        std::vector<Halfedge*> neighbors3 = vertexTemp->allHalfedgesInLoopCCW();
        std::cout<<"--------------------"<<std::endl;
        std::vector<Halfedge*> neighbors2 = vertexTemp->allOutgoingHalfedges();
        std::cout<<"--------------------"<<std::endl;

        std::cout<<"number neibors: "<<neighbors.size()<<std::endl;
        std::cout<<"number neibors: "<<neighbors3.size()<<std::endl;
        std::cout<<"number neibors outgoing: "<<neighbors2.size()<<std::endl;
        */
/*
        {   
            HalfedgeDS halfedgeDS02=halfedgeDS;
            //auto faceTemp= halfedgeDS.getFace(0);
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


       
       // Modificators::catmullClarkExp(halfedgeDS);
       // Modificators::catmullClarkExp(halfedgeDS);
       //   Modificators::catmullClarkExp(halfedgeDS);

       // Modificators::quadSubDivideStruct(halfedgeDS);


        //Operations::removeVertex(halfedgeDS, vertexTemp);

        //Operations::processFaceRecursive(halfedgeDS, faceTemp, 5);
        //Operations::processFaceRecursiveOppositeEdges(halfedgeDS, faceTemp, 11);
/*
        processRandomFace(halfedgeDS, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 1);
        processRandomFace(halfedgeDS, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 1);
        processRandomFace(halfedgeDS, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 2);
        processRandomFace(halfedgeDS, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 2);
        processRandomFace(halfedgeDS, 1, 0.95, Operations::processFaceRecursiveOppositeEdges, 2);

        processRandomDeleteFace(halfedgeDS, 1, 0.45f);
        processRandomDeleteHalfedges(halfedgeDS, 1, 0.015f);
*/
        std::cout<<"HEDS number faces: "<<halfedgeDS.getFaces().size()<<std::endl;
        std::cout<<"HEDS number vertex: "<<halfedgeDS.getVertices().size()<<std::endl;
        std::cout<<"HEDS number halfEdges: "<<halfedgeDS.getHalfedges().size()<<std::endl;


       // auto v1=Operations::addVertex( halfedgeDS, glm::vec3(0,0,0),true);
       // auto v2=Operations::addVertex( halfedgeDS, glm::vec3(1,0,0),true);
       // auto edge1=Operations::addEdge(halfedgeDS,v1,v2);
    }


    void Scene::testNautilus(HalfedgeDS& halfedgeDS)
    {
        std::vector<glm::vec3> sourcePoints;
        int N = 20; // Number of points
        float radius = 12.0f; // Radius of the half-circle
        fillSpiralPointsNAU(sourcePoints, N, 1.5,12.5, 5, 2.75);
        auto protoCube=Operations::generateTubeNL(sourcePoints,0.25,18,0,0);

        //auto protoCube=Operations::generateQuadArraysCube(3,3,3,1.0);
        auto protoCubeUniqVertex=Operations::computeUniquePositionsArray(protoCube.positions,protoCube.cells);

        Operations::setFromGeometry(halfedgeDS,protoCubeUniqVertex.positions, protoCubeUniqVertex.cells);
        //Operations::setFromGeometry(halfedgeDS,protoCube.positions, protoCube.cells);

        //halfedgeDS.mergeFrom(temphalfedgeDS);

        std::cout<<"HEDS number faces: "<<halfedgeDS.getFaces().size()<<std::endl;
        std::cout<<"HEDS number vertex: "<<halfedgeDS.getVertices().size()<<std::endl;
        std::cout<<"HEDS number halfEdges: "<<halfedgeDS.getHalfedges().size()<<std::endl;

    }

     void Scene::fillHalfCirclePoints(std::vector<glm::vec3>& sourcePoints, int N, float radius) {
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

    void Scene::fillSpiralPoints(std::vector<glm::vec3>& sourcePoints, int N, float initialRadius, float finalRadius, int numberOfLoops) {
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

    void Scene::fillSpiralPointsNAU(std::vector<glm::vec3>& sourcePoints, int N, float initialRadius, float finalRadius, int numberOfLoops, float radiusCoeff) {
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




    


}