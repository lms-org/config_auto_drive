#ifndef PHOENIX_CC2016_SERVICE_H
#define PHOENIX_CC2016_SERVICE_H

#include <lms/service.h>
#include <lms/extra/time.h>
namespace phoenix_CC2016_service {

enum class RemoteControlState{
    DISCONNECTED,IDLE,ACTIVE

};
enum class CCDriveMode{
    IDLE,PARKIN, FOH, FMH
};

/**
 * @brief LMS service phoenix_CC2016_service
 **/
class Phoenix_CC2016Service : public lms::Service {
    RemoteControlState m_state;
    RemoteControlState m_oldState;
    CCDriveMode m_driveMode;
    /**
     * @brief batteryVolate in mv
     */
    int m_batteryVoltage;
    lms::extra::PrecisionTime m_lastUpdate;

public:
    void update(RemoteControlState rcState, CCDriveMode driveMode, int batteryVoltage);
    void updateRcState(RemoteControlState state);
    bool rcStateChanged() const;
    RemoteControlState rcState() const;
    CCDriveMode driveMode() const;
    int batteryVoltage() const;
    lms::extra::PrecisionTime lastUpdate() const;
    /**
     * @brief isValid
     * @return true if the lastUpdate was in time
     */
    bool isValid() const;
    //system-methods
    bool init() override;
    void destroy() override;
};

} // namespace phoenix_CC2016_service

#endif // PHOENIX_CC2016_SERVICE_H
