#include "networkagent.h"
#include "ui/ui_manager.hpp"
#include "cs_protocol_def.hpp"
#include "commondef.hpp"
#include "windows.h"
#include "ui/mainwindow.h"

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
        UIManager::GetInstance().GetMainView()->SetTipsTxt("连接中央服务器成功，查询操作服务器IP端口！");

        Protocol::CSCheckServiceInfo check_info;
        check_info.service_type = ServiceType_GAME_OPERATOR;
        this->SendToServer(check_info);
    }
    else
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("连接操作服务器成功，查询游戏列表！");

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

    if (server_ip_ == SERVER_CENTER_IP_ADDR && server_port_ == SERVER_CENTER_LISTEN_PORT)
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("与中央服务器断开，开始连接操作服务器！");
        return;
    }

    UIManager::GetInstance().GetMainView()->SetTipsTxt("与操作服务器断开，准备重连！");
    Sleep(1000);

    this->ConnectToServer();
}

void NetworkAgent::ConnectToServer(IPAddr ip, Port port)
{
    if (0 != port)
    {
        server_ip_ = ip;
        server_port_ = port;
    }
    net_mgr_.SyncConnect(server_ip_, server_port_);
}

void NetworkAgent::Disconnect()
{
    net_mgr_.Disconnect();
}

void NetworkAgent::SendToServer(const face2wind::SerializeBase &data)
{
    net_mgr_.SendSerialize(data);
}
