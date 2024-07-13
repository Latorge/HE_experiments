#pragma once

#include "iostream"
#include "lib/Core/Struct.hpp"
#include "lib/Operations/Operations.hpp"
#include "lib/Operations/createOperations.hpp"
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

    };
}
