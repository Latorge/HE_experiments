#pragma once

#include "iostream"
#include "lib/Core/Struct.hpp"
#include "lib/Operations/Operations.hpp"
#include "lib/CreateOperations/createOperations.hpp"
#include "lib/DrawSupport/drawSupport.hpp"
#include "lib/Core/HalfedgeDS.hpp"



namespace HalfedgeLib {

    class FrontClass
    {
        public:
            FrontClass(std::string _name, int _id):halfedgeDS01(_name, _id) {}
            HalfedgeDS halfedgeDS01;

            void testCube(HalfedgeDS& halfedgeDS);
            std::vector<DrawSupport::PointInfo> getLinesfromHEDS(HalfedgeDS& halfedgeDS);

            DrawSupport::GeometryData getTrianglesfromHEDS(HalfedgeDS &halfedgeDS);
            void processRandomDeleteHalfedges(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff);
            void processRandomDeleteFace(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff);
            void processRandomFace(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff, std::function<void(HalfedgeDS &, Face *, int)> func, int iterations);
            template <typename Func, typename... Args>
            void processRandomFaceT(HalfedgeDS &halfedgeDS, int divideCoeff, float rndCoeff, Func func, Args &&...args);
    };
}
