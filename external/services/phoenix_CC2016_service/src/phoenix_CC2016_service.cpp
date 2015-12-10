#include "phoenix_CC2016_service/phoenix_CC2016_service.h"

namespace phoenix_CC2016_service {

bool Phoenix_CC2016Service::init() {
    //set default values
    m_state = RemoteControlState::DISCONNECTED;
    m_oldState = RemoteControlState::DISCONNECTED;
    m_driveMode = CCDriveMode::IDLE;
    m_batteryVoltage = 8;
    m_lastUpdate = lms::extra::PrecisionTime::ZERO;
    return true;
}

void Phoenix_CC2016Service::update(RemoteControlState rcState, CCDriveMode driveMode, int batteryVoltage){
    updateRcState(rcState);
    m_driveMode = driveMode;
    m_batteryVoltage = batteryVoltage;
    m_lastUpdate = lms::extra::PrecisionTime::now();
}

void Phoenix_CC2016Service::updateRcState(RemoteControlState state){
    m_oldState = m_state;
    m_state = state;
}

bool Phoenix_CC2016Service::rcStateChanged() const{
    return m_oldState != m_state;
}

RemoteControlState Phoenix_CC2016Service::rcState() const{
    return m_state;
}
CCDriveMode Phoenix_CC2016Service::driveMode() const{
    return m_driveMode;
}
int Phoenix_CC2016Service::batteryVoltage() const{
    return m_batteryVoltage;
}
lms::extra::PrecisionTime Phoenix_CC2016Service::lastUpdate() const{
    return m_lastUpdate;
}

void Phoenix_CC2016Service::destroy() {
    //Don't worry about me, I am just cleaning
}

} // namespace phoenix_CC2016_service
