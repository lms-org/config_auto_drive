#include <socket_data_sender/sender.h>
#include <lms/datamanager.h>
#include <string>
#include <socket_data/message_types.h>
#include <lms/extra/string.h>
bool Sender::cycle(){
    server->cycle();
    return true;
}

bool Sender::initialize(){
    server = new socket_connection::SocketServer(&logger);
    server->setSocketListener(this);
    server->start(getConfig()->get<int>("port",65111));
    return true;
}

bool Sender::deinitialize(){
    //TODO
    return false;
}

void Sender::receivedMessage(socket_connection::SocketConnector &from, char* buff, int bytesRead){
    char type = buff[0];
    logger.debug("Sender::receivedMessage") << "size: " <<bytesRead << "type: " << (int)type;
    switch ((MessageType)((int)type)) {
    case MessageType::CHANNEL_DATA:
        break;
    case MessageType::MESSAGE:
        break;
    case MessageType::CHANNEL_MAPPING:
        break;
    case MessageType::ERROR:
        break;
    case MessageType::GET_CHANNEL_DATA:
        //send the given channel to the client
        sendChannelToClient(from,buff[1]);
        break;
    case MessageType::GET_CHANNEL_DATA_ALL:
        sendChannelsToClient(from);
        break;
    case MessageType::REGISTER_CHANNEL:
        //TODO does that really add an element if it doesn't exists? :D
        logger.error("register channels: ") << "clientID: " << from.getID();
        std::vector<char> &clientMapping = clientChannels[from.getID()];

        //split the string
        std::vector<std::string> channels = lms::extra::split(&buff[1],bytesRead-1,';');
        if(channels.size() == 0){
            logger.error("register channels: ") << "NO CHANNELS RECEIVED!";
            //no channels received!
            break;
        }

        std::string ans(1,(char)MessageType::CHANNEL_MAPPING);
        for(std::string &channel:channels){
            char channelID = addChannel(channel);
            clientMapping.push_back(channelID);
            logger.debug("REGISTER_CHANNEL") <<"NAME-"<< channel<<"---";
            ans += channel+";"+ channelID+";";
        }
        from.sendMessage(ans.c_str(),ans.length(),true);
        break;
    }
}

void Sender::sendChannelsToClient(socket_connection::SocketConnector &from){
    std::vector<char> &clientMapping = clientChannels[from.getID()];
    if(clientMapping.size() == 0){
        logger.error("Client tried to get channels but has no registered!") <<"clientID: " <<from.getID();
    }
    for(char channelID : clientMapping){
        sendChannelToClient(from,channelID);
    }

}

void Sender::sendChannelToClient(socket_connection::SocketConnector &from,char channelId){
    //TODO error checking
    //serialize channel and send it
    std::ostringstream osstream;
    //first byte is the typeID
    char c = (char)MessageType::CHANNEL_DATA;
    osstream.write(&c,1);
    //second byte is the channelID
    osstream.write(&channelId,1);
    //write the data into the stream
    datamanager()->serializeChannel(this,channelMapping[channelId].name,osstream);
    from.sendMessage(osstream.str().c_str(),osstream.str().length(),true);
}

char Sender::addChannel(std::string name){
    //check if channel is already registered
    for(int i = 0; i < channelMapping.size(); i++){
        if(channelMapping[i].name == name){
            return i;
        }
    }
    ChannelMapping cm;
    cm.name = name;
    cm.iD = channelMapping.size();
    channelMapping.push_back(cm);
    logger.debug("added Channel") << "name,id" << name <<","<< (int)cm.iD;
    datamanager()->getReadAccess(this,name);
    return channelMapping.size() -1;
}

void Sender::disconnected(const socket_connection::SocketConnector &disconnected){
    //TODO
}

void Sender::connected(const socket_connection::SocketConnector &connected){
    //TODO
}
