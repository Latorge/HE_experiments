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
            void intersectionAABB(HalfedgeDS &halfedgeDS);
            std::vector<DrawSupport::PointInfo> getLinesfromHEDS(HalfedgeDS &halfedgeDS, bool onlyBoundaryLines, bool drawArrows);

            DrawSupport::GeometryData getTrianglesfromHEDS(HalfedgeDS &halfedgeDS);
            void quadSubDivideHEDS(HalfedgeDS &halfedgeDS);
            void catmullClarkHEDS(HalfedgeDS &halfedgeDS);
            void processFaceRecursiveOppositeEdges(HalfedgeDS &halfedgeDS, float rndCoeff=0.5, int recursiveDepth=2);
            void randomDeleteFaces(HalfedgeDS &halfedgeDS, float rndCoeff);
            void randomExtrudeFaces(HalfedgeDS &halfedgeDS, float rndCoeff, float distExtrude, float amountExtrude);
            void processRandomDeleteHalfedges(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff);
            void processRandomDeleteFace(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff);
            void processRandomFace(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff, std::function<void(HalfedgeDS &, Face *, int)> func, int iterations);
            template <typename Func, typename... Args>
            void processRandomFaceT(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff, Func func, Args &&...args);
    };
}
