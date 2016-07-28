#include <cmath>
#include <algorithm>
#include "car_to_matlab.h"
#include "street_environment/car.h"
#include <string>
#include <sstream>

bool CarToMatlab::initialize() {
    car = readChannel<street_environment::Car>("CAR");
    server = new socket_connection::SocketConnectionHandler(logger);
    server->openPortForRequests(config().get<int>("port",55555));
    return true;
}


bool CarToMatlab::deinitialize() {
    return true;
}

bool CarToMatlab::cycle() {
    logger.debug("cycle")<< car->position().x <<" "<< car->position().y<< " "<<car->targetSpeed()<<" "<<car->steeringFront()<<" "<<car->steeringRear();
    server->cycle();
    server->setSocketListener(this);

    std::stringstream ss;
    ss<<"car,xy:"<<car->position().x <<","<<car->position().y<<"#";
    ss<<"velocity:"<<car->velocity()<<"#";
    ss<<"test_value:" << rand()%10 << "#";

    if(server->hasConnections()){
        server->sendMessageToAllConnections(ss.str().c_str(),ss.str().size(),false);
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
