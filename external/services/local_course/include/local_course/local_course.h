#ifndef LOCAL_COURSE_H
#define LOCAL_COURSE_H

#include <lms/service.h>
#include "lms/math/vertex.h"
#include <vector>

namespace local_course {

/**
 * @brief LMS service local_course
 **/
class LocalCourse : public lms::Service {
    std::vector<lms::math::vertex2f> pointsToAdd;
public:
    bool init() override;
    void destroy() override;

    void update(float d, float dy, float dphi);
    void addPoints(const std::vector<lms::math::vertex2f> &points);
    void addPoint(const lms::math::vertex2f &p);
    void getCourse();
    void getCourse(lms::Time time);
};

} // namespace local_course

#endif // LOCAL_COURSE_H
