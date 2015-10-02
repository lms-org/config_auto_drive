#include <cmath>
#include <algorithm>
#include "car_to_matlab.h"
#include "lms/datamanager.h"
#include "sensor_utils/car.h"
#include <string>
#include <sstream>

bool CarToMatlab::initialize() {
    car = datamanager()->readChannel<sensor_utils::Car>(this,"CAR");
    server = new socket_connection::SocketServer(logger);
    server->start(getConfig()->get<int>("port",55555));
    return true;
}


bool CarToMatlab::deinitialize() {
    return true;
}

bool CarToMatlab::cycle() {
    logger.debug("cycle")<< car->position().x <<" "<< car->position().y<< " "<<car->targetSpeed<<" "<<car->steering_front<<" "<<car->steering_rear;
    server->cycle();
    server->setSocketListener(this);

    std::stringstream ss;
    ss<<"car,xy:"<<car->position().x <<","<<car->position().y<<"#";
    ss<<"velocity:"<<car->velocity()<<"#";
    ss<<"test_value:" << rand()%10 << "#";

    if(server->hasClients()){
        server->sendMessageToAllClients(ss.str().c_str(),ss.str().size(),false);
    }

    return true;
}
void CarToMatlab::disconnected(const socket_connection::SocketConnector &disconnected){
    (void)disconnected;
    //TODO
}

void CarToMatlab::connected(const socket_connection::SocketConnector &connected){
    (void)connected;
    //TODO
}
void CarToMatlab::receivedMessage(socket_connection::SocketConnector &from, char* buff, int bytesRead){
    (void)from;
    (void)buff;
    (void)bytesRead;
}
