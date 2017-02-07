#include "networkagent.h"
#include "ui/ui_manager.hpp"
#include "cs_protocol_def.hpp"
#include "commondef.hpp"

NetworkAgent::NetworkAgent() : server_ip_(""), server_port_(0), has_connected_(false)
{
    net_mgr_.RegistSerializeHandler(this);
    this->ConnectToServer();
}

NetworkAgent & NetworkAgent::GetInstance()
{
    static NetworkAgent instance;
    return instance;
}

void NetworkAgent::OnConnect(IPAddr ip, Port port, Port local_port, bool success)
{
    has_connected_ = true;

    if (ip == SERVER_CENTER_IP_ADDR && port == SERVER_CENTER_LISTEN_PORT)
    {
        Protocol::CSCheckServiceInfo check_info;
        check_info.service_type = ServiceType_GAME_OPERATOR;
        this->SendToServer(check_info);
    }
    else
    {
        qDebug()<<"connect to game server ";
        Protocol::CSGORequestCommandList req_command_list;
        this->SendToServer(req_command_list);
    }
}

void NetworkAgent::OnRecv(const face2wind::SerializeBase *data)
{
    msg_handler_.OnRecv(data);
}

void NetworkAgent::OnDisconnect()
{
    has_connected_ = false;
    UIManager::GetInstance().ShowLogin();
    this->ConnectToServer();
}

void NetworkAgent::ConnectToServer(IPAddr ip, Port port)
{
    if (0 == port)
    {
        net_mgr_.SyncConnect(server_ip_, server_port_);
    }
    else
    {
        net_mgr_.SyncConnect(ip, port);
        server_ip_ = ip;
        server_port_ = port;
    }
}

void NetworkAgent::Disconnect()
{
    net_mgr_.Disconnect();
}

void NetworkAgent::SendToServer(const face2wind::SerializeBase &data)
{
    net_mgr_.SendSerialize(data);
}
