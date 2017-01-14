#include "networkagent.h"
#include "ui/ui_manager.hpp"
#include "cs_protocol_def.hpp"

NetworkAgent::NetworkAgent() : server_ip_("192.168.11.199"), server_port_(52013), has_connected_(false)
{
    net_mgr_.RegistSerializeHandler(this);
    net_mgr_.SyncConnect(server_ip_, server_port_);
}

NetworkAgent & NetworkAgent::GetInstance()
{
    static NetworkAgent instance;
    return instance;
}

void NetworkAgent::OnConnect(IPAddr ip, Port port, Port local_port, bool success)
{
    has_connected_ = true;

    //Protocol::CSCheckServiceInfo
}

void NetworkAgent::OnRecv(const face2wind::SerializeBase *data)
{
    msg_handler_.OnRecv(data);
}

void NetworkAgent::OnDisconnect()
{
    has_connected_ = false;
    UIManager::GetInstance().ShowLogin();
   net_mgr_.SyncConnect(server_ip_, server_port_);
}

void NetworkAgent::ConnectToServer()
{
    net_mgr_.SyncConnect(server_ip_, server_port_);
}

void NetworkAgent::SendToServer(const face2wind::SerializeBase &data)
{
    net_mgr_.SendSerialize(data);
}
