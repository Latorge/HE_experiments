#include "front_class.hpp"
#include "iostream"

namespace HalfedgeLib {

    void FrontClass::testCube(HalfedgeDS& halfedgeDS)
    {

        //auto protoCube=Operations::generateQuadArraysCube(3,3,3,1.0);
        auto protoCube=Operations::generateQuadArrays(1,1,3.0);

        for (auto& pos : protoCube.positions) {
            std::cout << "Vertex Coordinates: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;

        }
        auto protoCubeUniqVertex=Operations::computeUniquePositionsArray(protoCube.positions,protoCube.cells);

         for (auto& pos : protoCubeUniqVertex.positions) {
            std::cout << "Vertex Coordinates: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;

        }
        Operations::setFromGeometry(halfedgeDS01,protoCubeUniqVertex.positions, protoCubeUniqVertex.cells);
        //Operations::setFromGeometry(halfedgeDS01,protoCube.positions, protoCube.cells);

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
        /*

        for(auto he: halfedgeDS01.getHalfedges())
        {
            glm::vec3 v1=he->vertex->position;
            glm::vec3 v2=he->twin->vertex->position;

            DrawSupport::PointInfo DrawSupport::PointInfo1;
            DrawSupport::PointInfo1.position=v1;
            DrawSupport::PointInfo1.color=glm::vec4(0.5,1,1,1);
            resultList.push_back(DrawSupport::PointInfo1);

            DrawSupport::PointInfo DrawSupport::PointInfo2;
            DrawSupport::PointInfo2.position=v2;
            DrawSupport::PointInfo2.color=glm::vec4(1,1,0.5,1);
            resultList.push_back(DrawSupport::PointInfo2);
        }
        */
        resultList=DrawSupport::setHalgEdgesLines(halfedgeDS01);
        return resultList;
    }
}
