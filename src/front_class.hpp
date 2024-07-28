#pragma once

#include "iostream"
#include "lib/Core/Struct.hpp"
#include "lib/Operations/Operations.hpp"
#include "lib/CreateOperations/createOperations.hpp"
#include "lib/Modificators/Modificators.hpp"
#include "lib/Intersection/intersections.hpp"
#include "lib/DrawSupport/drawSupport.hpp"
#include "lib/Core/HalfedgeDS.hpp"



namespace HalfedgeLib {

    class FrontClass
    {
        public:
            FrontClass(std::string _name, int _id):halfedgeDS01(_name, _id) {}
            HalfedgeDS halfedgeDS01;

            void testCube(HalfedgeDS& halfedgeDS);
            void testNautilus(HalfedgeDS &halfedgeDS);
            void intersectionAABB(HalfedgeDS &halfedgeDS);
            void fillHalfCirclePoints(std::vector<glm::vec3> &sourcePoints, int N, float radius);
            void fillSpiralPoints(std::vector<glm::vec3> &sourcePoints, int N, float initialRadius, float finalRadius, int numberOfLoops);
            void fillSpiralPointsNAU(std::vector<glm::vec3> &sourcePoints, int N, float initialRadius, float finalRadius, int numberOfLoops, float radiusCoeff);
            std::vector<DrawSupport::PointInfo> getLinesfromHEDS(HalfedgeDS &halfedgeDS, bool onlyBoundaryLines, bool drawArrows);

            DrawSupport::GeometryData debugLines;

            DrawSupport::GeometryData getTrianglesfromHEDS(HalfedgeDS &halfedgeDS);
            void triangleSubDivideHEDS(HalfedgeDS &halfedgeDS);
            DrawSupport::GeometryData getDebugLines();

            std::vector<Face*> selectedFaces;
            
            void quadSubDivideHEDS(HalfedgeDS &halfedgeDS);
            void catmullClarkHEDS(HalfedgeDS &halfedgeDS);
            void processFaceRecursiveOppositeEdges(HalfedgeDS &halfedgeDS, float rndCoeff=0.5, int recursiveDepth=2);
            void randomDeleteFaces(HalfedgeDS &halfedgeDS, float rndCoeff);
            void randomExtrudeFaces(HalfedgeDS &halfedgeDS, float rndCoeff, float distExtrude, float amountExtrude);
            void clearSelectedFaces();
            void selectedExtrudeFaces(HalfedgeDS &halfedgeDS, std::vector<Face *> selectedFaces, float distExtrude, float amountExtrude);

            void processRandomDeleteHalfedges(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff);
            void processRandomDeleteFace(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff);
            void processRandomFace(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff, std::function<void(HalfedgeDS &, Face *, int)> func, int iterations);
            std::vector<Face *> selectRandomFaces(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff);
            template <typename Func, typename... Args>
            void processRandomFaceT(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff, Func func, Args &&...args);
            template <typename Func, typename... Args>
            void processSelectedFaceT(HalfedgeDS &halfedgeDS, std::vector<Face *> selectedFaces, Func func, Args &&...args);
    };
}
