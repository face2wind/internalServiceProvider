#ifndef NETWORKAGENT_H
#define NETWORKAGENT_H

#include "memory/serialize/serialize_manager.hpp"
#include "msg_handler.h"

class NetworkAgent : public face2wind::ISerializeNetworkHandler
{
public:
    NetworkAgent();
    static NetworkAgent & GetInstance();

    virtual void OnConnect(IPAddr ip, Port port, Port local_port, bool success);
    virtual void OnRecv(const face2wind::SerializeBase *data);
    virtual void OnDisconnect();

    bool HasConnected() { return has_connected_; }

    void ConnectToServer();
    void SendToServer(const face2wind::SerializeBase &data);

private:
    IPAddr server_ip_;
    Port server_port_;

    bool has_connected_;
    MessageHandler msg_handler_;

    face2wind::SerializeNetworkManager net_mgr_;
};

#endif // NETWORKAGENT_H
