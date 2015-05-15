#include "trajectory_point_follower.h"
#include <cmath>
bool TrajectoryLineFollower::initialize() {
    controlData = datamanager()->writeChannel<Comm::SensorBoard::ControlData>(this,"CONTROL_DATA");
    trajectoryPoint = datamanager()->writeChannel<lms::math::vertex<4,float>>(this,"POINT");

    return true;
}

bool TrajectoryLineFollower::deinitialize() {
    return true;
}

bool TrajectoryLineFollower::cycle() {
    /*
    std::pair<float,float> steering = TobisRegler((*trajectoryPoint)[0], (*trajectoryPoint)[1],
            atan2((*trajectoryPoint)[3], (*trajectoryPoint)[2]), 0.5);
    */

    std::pair<float,float> steering = smartRegler((*trajectoryPoint)[0], (*trajectoryPoint)[1],
            atan2((*trajectoryPoint)[3], (*trajectoryPoint)[2]));

    controlData->vel_mode = Comm::SensorBoard::ControlData::MODE_VELOCITY;
    controlData->steering_front = steering.first;// * 180. / M_PI;
    controlData->steering_rear = steering.second;// * 180. / M_PI;
    controlData->control.velocity.velocity = 0.5;
    return true;
}

std::pair<float,float> TrajectoryLineFollower::smartRegler(float x, float y, float phi){

    logger.info("input_data: ") << y << " " << phi;
    float delta_v = 3*y;
    float delta_h = phi;
    return std::make_pair(delta_v, delta_h);
}


std::pair<float,float> TrajectoryLineFollower::TobisRegler(double x_s, double y_s, double phi_s, double v) {

    logger.info("input_data: ") << x_s << " " << y_s << " " << phi_s << " " << v;
    // Optimal model predictive controller; Herleitung in Matlab durch Tobias Brunner
    // Endzeitpunkt berechnen
    double te_n = (std::abs(x_s) + std::abs(y_s) + sqrt(pow(x_s,2)+pow(y_s,2)))/(2*v);
    // Hinterer Lenkwinkel
        // Konstanten
            double b01 = 1.7182708618947581461128704937992e66;
            double b02 = 5.3238409842306255217304803428297e66;
            double b03 = 6.1573551603729003160957612906846e65;
            double b04 = 4.6114120701330902104516415523344e66;
            double b05 = 5.533205866333774578750664824624;
            double b06 = 3.8977773824240266798699997681664e66;
            double b07 = 11.066411732667549157501329649248;
            double b08 = 6.2034834174905717850958205443336e66;
            double b09 = 3.5408626083187939226144612852791;
            double b10 = 7.5255491243487552348868683639689;
        // Zusammengefasste Ausdruecke
            double a1 = exp(3.7627745621743776174434341819845*te_n*std::abs(v)); // Laut Matlab Wurzel aus Quadrat; Rueckwaertsfahrt?
            double a2 = exp(7.3036371704931715400578954672635*te_n*std::abs(v));
            double a3 = exp(1.7704313041593969613072306426395*te_n*std::abs(v));
            double a4 = exp(9.2959804285081521961940990066084*te_n*std::abs(v));
        // Formel
            double delta_h = -(
                                phi_s*pow(v,2)*(b01*a4+b01*a3-b01*a2-b01*a1)
                                - v*y_s*std::abs(v)*(b02*a4-b02*a3-b03*a2+b03*a1)
                            )/
                            (
                                pow(v,2)*(b04*exp(b05*te_n*std::abs(v))
                                +b06*exp(b07*te_n*std::abs(v))
                                - b08*exp(b09*te_n*std::abs(v))
                                - b08*exp(b10*te_n*std::abs(v))
                                +b06)
                            );
    // Vorderer Lenkwinkel
        // Konstanten
            double d01 = 243611086401501667491874985510400.0;
            double d02 = 133310649738314984359366082060588.0;
            double d03 = 1.1526453845379406243436199954797e33;
            double d04 = 288213254383318138153227597020900.0;
            double d05 = 387717713593160736568488784020877.0;
            double d06 = 413045880264880861235354175131999.0;
            double d07 = 532384098423062552173048034282999.0;
            double d08 = 61573551603729003160957612906846.0;
            double d09 = 171827086189475814611287049379922.0;
            double d10 = 461141207013309021045164155233444.0;
            double d11 = 620348341749057178509582054433388.0;
            double d12 = 389777738242402667986999976816644.0;
        // Zusammengefasste Ausdruecke
            double c10 = exp(5.533205866333774578750664824624*te_n*std::abs(v));
            double c09 = exp(11.066411732667549157501329649248*te_n*std::abs(v));
            double c08 = exp(3.5408626083187939226144612852791*te_n*std::abs(v));
            double c07 = exp(7.5255491243487552348868683639689*te_n*std::abs(v));
            double c06 = exp(9.2959804285081521961940990066084*te_n*std::abs(v));
            double c05 = exp(1.7704313041593969613072306426395*te_n*std::abs(v));
            double c04 = exp(7.3036371704931715400578954672635*te_n*std::abs(v));
            double c03 = exp(3.7627745621743776174434341819845*te_n*std::abs(v));
            double c02 = d10*c10 + d12*c09 - d11*c08 - d11*c07 + d12;
            double c01 = (
                            d07*v*y_s*c06 - d07*v*y_s*c05 - d08*v*y_s*c04
                            + d08*v*y_s*c03 - d09*phi_s*c06*std::abs(v) - d09*phi_s*c05*std::abs(v)
                            +d09*phi_s*c04*std::abs(v) +d09*phi_s*c03*std::abs(v)
                        )
                         /
                        ( std::abs(v)*c02 );
        // Formel
            double delta_v = atan(
                            (1.6* (d01*sin(c01)*std::abs(v)
                            +d02*phi_s*v*c05
                            +d03*phi_s*v*c04
                            -d03*phi_s*v*c03
                            +d04*sin(c01)*c10*std::abs(v)
                            +d01*sin(c01)*c09*std::abs(v)
                            -d05*sin(c01)*c08*std::abs(v)
                            -d05*sin(c01)*c07*std::abs(v)
                            +d06*y_s*c06*std::abs(v)
                            +d06*y_s*c05
                            -d06*y_s*c04
                            -d06*y_s*c03
                            -d02*phi_s*v*c06)
                            )
                             /
                            ( cos(c01)*std::abs(v)*c02)
                            );
            delta_v *=-1;
            logger.info("RESULT: ")<<"vorne: " << delta_v << " hinten: " <<delta_h;
    return std::make_pair((float)delta_v, (float)delta_h);
}
