#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <lms/imaging/warp.h>
#include "lms/math/vertex.h"
#include "lms/math/polyline.h"
#include "street_environment/street_environment.h"

namespace street_environment {

    enum class RoadLaneType {
        LEFT, MIDDLE, RIGHT
    };

    class RoadLane:public lms::math::polyLine2f,public EnvironmentObject{
        RoadLaneType m_type;
    public:
        /**
         * @brief polarDarstellung
         * polarDarstellung[0] is the y-deviance
         * polarDarstellung[1] is the start-angle in rad
         * polarDarstellung[>1] Krümmungen relativ zum vorherigen stück
         */
        std::vector<double> polarDarstellung; //TODO english name :)
        float polarPartLength;
        RoadLaneType type() const{
            return m_type;
        }

        void type(RoadLaneType type){
            m_type = type;
        }
    };


    typedef std::shared_ptr<RoadLane> RoadLanePtr;
}

#endif /* ENVIRONMENT_H */
