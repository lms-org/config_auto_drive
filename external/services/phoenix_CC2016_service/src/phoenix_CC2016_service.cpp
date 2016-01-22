#include "phoenix_CC2016_service/phoenix_CC2016_service.h"

namespace phoenix_CC2016_service {

bool Phoenix_CC2016Service::init() {
    //set default values
    m_state = RemoteControlState::DISCONNECTED;
    m_oldState = RemoteControlState::DISCONNECTED;
    m_driveMode = CCDriveMode::IDLE;
    m_batteryVoltage = 8;
    m_lastUpdate = lms::Time::ZERO;
    if(config().get<bool>("stateFromConfig",false))
        updateFromConfig();
    return true;
}
void Phoenix_CC2016Service::updateFromConfig(){
    //call it twice to set rcStateChanged to false
    update(RemoteControlState::DISCONNECTED,CCDriveMode::FMH,8);
    update(RemoteControlState::DISCONNECTED,CCDriveMode::FMH,8);
    logger.error("updateFromConfig");
}

void Phoenix_CC2016Service::update(RemoteControlState rcState, CCDriveMode driveMode, int batteryVoltage){
    updateRcState(rcState);
    m_driveMode = driveMode;
    m_batteryVoltage = batteryVoltage;
    m_lastUpdate = lms::Time::now();
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
lms::Time Phoenix_CC2016Service::lastUpdate() const{
    return m_lastUpdate;
}

bool Phoenix_CC2016Service::isValid() const{
    //std::cout<<"isValid Ph-Service: "<<lms::extra::PrecisionTime::since(m_lastUpdate).toFloat<std::milli>()<<std::endl;
    if(config().get<int>("updateInMilliS", 100) == -1){
        return true;
    }
    return lms::Time::since(m_lastUpdate).toFloat<std::milli>()< config().get<int>("updateInMilliS", 100);
}

void Phoenix_CC2016Service::destroy() {
    //Don't worry about me, I am just cleaning
}

} // namespace phoenix_CC2016_service
