#ifndef NETWORKAGENT_H
#define NETWORKAGENT_H

#include "elegance/memory/serialize/serialize_manager.hpp"
#include "msg_handler.h"

using namespace face2wind;

class NetworkAgent : public face2wind::ISerializeNetworkHandler
{
public:
    NetworkAgent();
    static NetworkAgent & GetInstance();

    virtual void OnListenFail(Port port) {}
    virtual void OnAccept(IPAddr ip, Port port, Port local_port, NetworkID net_id) {}

    virtual void OnConnect(IPAddr ip, Port port, Port local_port, bool success, NetworkID net_id);
    virtual void OnRecv(NetworkID net_id, const face2wind::SerializeBase *data);
    virtual void OnDisconnect(NetworkID net_id);

    void ConnectToServer(IPAddr ip, Port port);
    void Disconnect();
    void SendToServer(const face2wind::SerializeBase &data);

private:
    NetworkID server_netid_;

    IPAddr server_ip_;
    Port server_port_;

    IPAddr center_server_ip_;
    Port center_server_port_;
    int service_type_;
    int project_list_type_;

    MessageHandler msg_handler_;

    face2wind::SerializeNetworkManager net_mgr_;
};

#endif // NETWORKAGENT_H
