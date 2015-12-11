#ifndef SENSELINK_CSVLOGGER_H
#define SENSELINK_CSVLOGGER_H

#include <lms/module.h>
#include <lms/extra/time.h>
#include <unordered_map>
#include <sense_link/sensors.h>

class senselink_csvlogger : public lms::Module {
public:
    bool initialize();
    bool deinitialize();
    bool cycle();
    
protected:
    void log( std::ofstream& os,
              sense_link::SensorType sensorType,
              const sense_link::SensorMeasurement& measurement );
    
    std::string generateFilename(sense_link::SensorType type, uint8_t id);
    std::ofstream& getLogfile(sense_link::SensorType type, uint8_t id);
    
    lms::extra::PrecisionTime offset;
    
    lms::ReadDataChannel<sense_link::Sensors> sensors;
    std::string prefix;
    std::unordered_map<std::pair<sense_link::SensorType, uint8_t>, std::ofstream> files;
    size_t cycleCounter;
};

#endif // SENSELINK_CSVLOGGER_H
