#include "createOperations.hpp"
#include <vector>

#include "spline.h"
#include "splines/natural_spline.h"
#include "splines/cubic_hermite_spline.h"
#include "splines/quintic_hermite_spline.h"
#include "splines/uniform_cubic_bspline.h"
#include "vector.h"

namespace Operations {
 
    ProtoMesh generateTube(std::vector<glm::vec3> sourcePoints, int numSegments, float startAngle, float endAngle)
    {
        ProtoMesh mesh;

        int number_of_interpolate=3;
        int number_of_segments=numSegments;

        float radius=1.0f;


        std::vector<glm::vec3> tube_vertices;
        std::vector<glm::vec3> tangents;

        std::vector<Vector3> splineSourceList;

        //create spline
        for (size_t i = 0; i < sourcePoints.size(); ++i)
        {
            Vector3 vtemp=Vector3(sourcePoints[i].x,sourcePoints[i].y,sourcePoints[i].z);
            splineSourceList.push_back(vtemp);
        }


        //LoopingCubicHermiteSpline<Vector3> sL(splineSourceList);
       NaturalSpline<Vector3> sL(splineSourceList);

       // Loop to create spline vertices
        // Assuming sL is properly initialized and can handle glm::vec3
        for (size_t i = 0; i < splineSourceList.size()-1; ++i) {
            for (int step = 0; step < number_of_interpolate; ++step) {
                float t = float(step) / number_of_interpolate;
                auto posV =  sL.getPosition(i + t);  // Correct use of getPosition
                auto tangV = sL.getTangent(i + t);  // Correct use of getTangent

                glm::vec3 pos =  glm::vec3(posV[0],posV[1],posV[2]);
                glm::vec3 tang = glm::vec3(tangV.tangent[0],tangV.tangent[1],tangV.tangent[2]);

                tube_vertices.push_back(pos);
                tangents.push_back(tang);
            }
        }

        // Generate tube vertices around each spline vertex
        glm::vec3 v_prev;
        bool isFirst = true;

        for (size_t i = 0; i <  tube_vertices.size(); ++i) {

            glm::vec3 v_prev;
            bool isFirst = true;
            auto tangent=tangents[i];

            for (int segment = 0; segment < number_of_segments; ++segment) {
               // float theta = startAngle + (endAngle - startAngle) * float(segment) / float(number_of_segments - 1);
                float theta = 2.0 * glm::pi<float>() * (static_cast<float>(segment) / static_cast<float>(number_of_segments));
                glm::vec3 u, v;
                if (isFirst) {
                    u = glm::normalize(glm::cross(glm::vec3(1, 1, 1), tangent));
                    isFirst = false;
                } else {
                    u = glm::normalize(glm::cross(tangent, v_prev));
                }
                v = glm::normalize(glm::cross(u, tangent));
                v_prev = v;

                float x = radius * cosf(theta);
                float y = radius * sinf(theta);
                glm::vec3 vertex = u * x + v * y + tube_vertices[i];
                mesh.positions.push_back(vertex);
            }
        }

        // Generate quads
        int halfSegmentCount=0;//segmentsNumber/2;
        int numberSplineSteps = tube_vertices.size();

        for (int ring_index = 0; ring_index < numberSplineSteps; ring_index++) {
            int next_ring_index = (ring_index + 1) % numberSplineSteps;

            for (int local_index = 0; local_index < number_of_segments; local_index++) {
                int next_local_index = (local_index + 1) % number_of_segments;
                int rlocal_index =      (halfSegmentCount+local_index) % (number_of_segments);

                int a = ring_index * number_of_segments + local_index;
                int b = ring_index * number_of_segments + (local_index +1)% (number_of_segments);
                int c = next_ring_index * number_of_segments + rlocal_index;
                int d = next_ring_index * number_of_segments + next_local_index;

                mesh.cells.push_back({a, c, d, b});
            }
        }

        return mesh;
    }

     ProtoMesh generateTubeNL(std::vector<glm::vec3> sourcePoints, float radius,int numSegments, float startAngle, float endAngle)
    {
        ProtoMesh mesh;

        int number_of_interpolate=3;
        int number_of_segments=numSegments;

       // float radiusCoeff=0.00125f;
        float radiusCoeff=0.00075f;
        float radiusCoeffSum=0.01f;


        std::vector<glm::vec3> tube_vertices;
        std::vector<glm::vec3> tangents;

        std::vector<Vector3> splineSourceList;

        //create spline
        for (size_t i = 0; i < sourcePoints.size(); ++i)
        {
            Vector3 vtemp=Vector3(sourcePoints[i].x,sourcePoints[i].y,sourcePoints[i].z);
            splineSourceList.push_back(vtemp);
        }


        //LoopingCubicHermiteSpline<Vector3> sL(splineSourceList);
       NaturalSpline<Vector3> sL(splineSourceList);

       // Loop to create spline vertices
        // Assuming sL is properly initialized and can handle glm::vec3
        for (size_t i = 0; i < splineSourceList.size()-1; ++i) {
            for (int step = 0; step < number_of_interpolate; ++step) {
                float t = float(step) / number_of_interpolate;
                auto posV =  sL.getPosition(i + t);  // Correct use of getPosition
                auto tangV = sL.getTangent(i + t);  // Correct use of getTangent

                glm::vec3 pos =  glm::vec3(posV[0],posV[1],posV[2]);
                glm::vec3 tang = glm::vec3(tangV.tangent[0],tangV.tangent[1],tangV.tangent[2]);

                tube_vertices.push_back(pos);
                tangents.push_back(tang);
            }
        }

        // Generate tube vertices around each spline vertex
        glm::vec3 v_prev;
        bool isFirst = true;
        float runningSpin=0;

        for (size_t i = 0; i <  tube_vertices.size(); ++i) {

            glm::vec3 v_prev;
           
            auto tangent=tangents[i];
            runningSpin=0;//3.0*2.0*glm::pi<float>()/tube_vertices.size();


            for (int segment = 0; segment < number_of_segments; ++segment) {
               // float theta = startAngle + (endAngle - startAngle) * float(segment) / float(number_of_segments - 1);
               // float theta = 2.0 * glm::pi<float>() * (static_cast<float>(segment) / static_cast<float>(number_of_segments));
                float theta =runningSpin + 2.0 * glm::pi<float>()* (static_cast<float>(segment) / static_cast<float>(number_of_segments));
                glm::vec3 up = glm::vec3(0, 0, 1);
                glm::vec3 u, v;
                if (isFirst) {
                    glm::vec3 crossResult = glm::cross(up, tangent);
                    float croosLenghtRes=glm::length(crossResult);
                    if (croosLenghtRes< 0.09f) {  // Threshold to detect near-zero cross product
                        up = glm::vec3(1, 0, 0);  // Choose a different up vector
                        crossResult = glm::cross(up, tangent);
                    }
                    u = glm::normalize(crossResult);
                    isFirst = false;
                } else {
                    u = glm::normalize(glm::cross(tangent, v_prev));
                }
                v = glm::normalize(glm::cross(u, tangent));
                v_prev = v;

                float x =(radius+radiusCoeffSum) * cosf(theta);
                float y =(radius+radiusCoeffSum) * sinf(theta);
                glm::vec3 vertex = u * x + v * y + tube_vertices[i];
                mesh.positions.push_back(vertex);

                radiusCoeffSum+=radiusCoeff;
            }
        }

        // Generate quads
        int halfSegmentCount=0;//segmentsNumber/2;
        int numberSplineSteps = tube_vertices.size();

        for (int ring_index = 0; ring_index < numberSplineSteps-1; ring_index++) {
            int next_ring_index = (ring_index + 1) % numberSplineSteps;

            for (int local_index = 0; local_index < number_of_segments; local_index++) {
                int next_local_index = (local_index + 1) % number_of_segments;
                int rlocal_index =      (halfSegmentCount+local_index) % (number_of_segments);

                int a = ring_index * number_of_segments + local_index;
                int b = ring_index * number_of_segments + (local_index +1)% (number_of_segments);
                int c = next_ring_index * number_of_segments + rlocal_index;
                int d = next_ring_index * number_of_segments + next_local_index;

                mesh.cells.push_back({a, c, d, b});
            }
        }

        return mesh;
    }
}
