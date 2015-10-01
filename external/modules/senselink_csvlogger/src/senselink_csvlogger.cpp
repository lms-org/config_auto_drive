#include "senselink_csvlogger.h"
#include <sense_link/utils.h>
#include <iomanip>

bool senselink_csvlogger::initialize() {
    logger.info("initialize") << "Open senselink_csvlogger";
    sensors = datamanager()->readChannel<sense_link::Sensors>(this, "SENSOR_DATA");
    prefix = getConfig()->get<std::string>("prefix") + "_" + lms::extra::currentTimeString();
    offset = lms::extra::PrecisionTime::now();
    return true;
}

bool senselink_csvlogger::deinitialize() {
    for(auto& f : files)
    {
        // Close all opened log files
        f.second.close();
    }
    logger.info("deinitialize") << "Close senselink_csvlogger";
    return true;
}

bool senselink_csvlogger::cycle() {
    for( const auto& list : *sensors )
    {
        // Get CSV file
        auto type = list.first.first;
        auto id = list.first.second;
        auto& logfile = getLogfile(type, id);
        
        logger.debug() << "List contains " << list.second.size() << " entries";
        for( const auto& measurement : list.second )
        {
            log( logfile, list.first.first, measurement );
        }
    }
    return true;
}

std::string senselink_csvlogger::generateFilename(sense_link::SensorType type, uint8_t id)
{
    std::string sensorName;
    if(!sense_link::Utils::sensorNameFromType(type, sensorName))
    {
        // Use numeric type
        std::ostringstream ostr;
        ostr << static_cast<size_t>( type );
        std::string sensorName = ostr.str();
    }
    
    std::ostringstream filename;
    filename << prefix << "_" << sensorName << "_" << size_t(id) << ".csv";
    return filename.str();
}

std::ofstream& senselink_csvlogger::getLogfile(sense_link::SensorType type, uint8_t id)
{
    auto key = std::make_pair(type, id);
    auto file = files.find(key);
    if(file == files.end())
    {
        // Create new log file
        std::string filename = generateFilename(type, id);
        files[key].open(filename);
    }
    
    return files[key];
}

void senselink_csvlogger::log(  std::ofstream& os,
                                sense_link::SensorType sensorType,
                                const sense_link::SensorMeasurement& measurement )
{
    auto timestamp = (measurement.timestamp - offset).micros();
    switch( sensorType )
    {
        case sense_link::SensorType::IMU:
            os
                << timestamp << ","
                << measurement.data.IMU.accelerometer.x << ","
                << measurement.data.IMU.accelerometer.y << ","
                << measurement.data.IMU.accelerometer.z << ","
                << measurement.data.IMU.gyroscope.x << ","
                << measurement.data.IMU.gyroscope.y << ","
                << measurement.data.IMU.gyroscope.z << ","
                << measurement.data.IMU.magnetometer.x << ","
                << measurement.data.IMU.magnetometer.y << ","
                << measurement.data.IMU.magnetometer.z
                << std::endl;
            break;
        case sense_link::SensorType::PROXIMITY:
            os
                << timestamp << ","
                << measurement.data.Proximity.distance
                << std::endl;
            break;
        case sense_link::SensorType::ACCELEROMETER:
            os
                << timestamp << ","
                << measurement.data.Accelerometer.x << ","
                << measurement.data.Accelerometer.y << ","
                << measurement.data.Accelerometer.z
                << std::endl;
            break;
        case sense_link::SensorType::GYROSCOPE:
            os
                << timestamp << ","
                << measurement.data.Gyroscope.x << ","
                << measurement.data.Gyroscope.y << ","
                << measurement.data.Gyroscope.z
                << std::endl;
            break;
        case sense_link::SensorType::MAGNETOMETER:
            os
                << timestamp << ","
                << measurement.data.Magnetometer.x << ","
                << measurement.data.Magnetometer.y << ","
                << measurement.data.Magnetometer.z
                << std::endl;
            break;
        case sense_link::SensorType::MOUSE:
        os
            << timestamp << ","
            << measurement.data.Mouse.x << ","
            << measurement.data.Mouse.y << ","
            << measurement.data.Mouse.surfaceQuality << ","
            << measurement.data.Mouse.duration
            << std::endl;
        case sense_link::SensorType::ODOMETER:
        os
            << timestamp << ","
            << measurement.data.Odometer.ticks << ","
            << measurement.data.Odometer.duration << ","
            << measurement.data.Odometer.direction
            << std::endl;
        break;
        default:
            logger.warn() << "Unhandled sensor type" << static_cast<size_t>(sensorType);
            break;
    }
}
