#ifndef CAR_TRACKER_H
#define CAR_TRACKER_H

#include "lms/module.h"
#include "comm/senseboard.h"
#include "lms/extra/time.h"
#include "sensor_utils/car.h"

#include <kalman/UnscentedKalmanFilter.hpp>
#include "filter/measurement_model.h"
#include "filter/system_model.h"


typedef float T;


/**
 * @brief The CarTracker class
 * tracks the motion of our rc car
 */
class CarTracker : public lms::Module {

public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;

private:
    State<T> x; // Simulated (true) system state
    Control<T> u; //Control input
    SystemModel<T> sys; // System model
    MeasurementModel<T> mm; // Measurement model

    Kalman::UnscentedKalmanFilter<State <T>> ukf; //UKF

    void updateFromVehicle();

    lms::extra::PrecisionTime last;
    bool firstRun;
    lms::WriteDataChannel<sensor_utils::Car> car;
};

#endif /* CAR_TRACKER */
