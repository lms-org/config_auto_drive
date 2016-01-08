#ifndef AREA_OF_DETECTION_H
#define AREA_OF_DETECTION_H

#include <lms/service.h>
#include "lms/math/math.h"
#include <functional>
namespace area_of_detection {

/**
 * @brief LMS service area_of_detection
 **/
class AreaOfDetection : public lms::Service {
    std::vector<lms::math::Rect> m_visibleAreas;
public:
    bool init() override;
    void destroy() override;
    void configsChanged() override;
    std::vector<lms::math::Rect> visibleAreas();
    //Warum muss man dort eine referenz und keine Kopie übergeben?
    std::vector<lms::math::Rect> visibleAreas(std::function<void(float,float,float&,float&)> convertPos);
};

} // namespace area_of_detection

#endif // AREA_OF_DETECTION_H
