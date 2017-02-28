#include "networkagent.h"
#include "ui/ui_manager.hpp"
#include "cs_protocol_def.hpp"
#include "commondef.hpp"
#include "ui/mainwindow.h"
#include <QFile>
#include "client_def.hpp"

NetworkAgent::NetworkAgent() : server_ip_(""), server_port_(0), center_server_ip_(""), center_server_port_(0), has_connected_(false), is_connecting_(false)
{
    net_mgr_.RegistSerializeHandler(this);

    QFile file_project_list("center_server_addr.txt");
    bool read_file_succ = false;

    if (file_project_list.open(QFile::ReadOnly))
    {
        QTextStream in(&file_project_list);
        QString ip_str;
        int port;
        in >> ip_str >> port;
        center_server_ip_ = ip_str;
        center_server_port_ = port;
        read_file_succ = true;
    }
    if (!read_file_succ)
    {
        center_server_ip_ = "192.168.11.199";
        center_server_port_ = 52023;
    }

    this->ConnectToServer(center_server_ip_, center_server_port_);
}

NetworkAgent & NetworkAgent::GetInstance()
{
    static NetworkAgent instance;
    return instance;
}

void NetworkAgent::OnConnect(IPAddr ip, Port port, Port local_port, bool success)
{
    has_connected_ = true;
    is_connecting_ = false;
    server_ip_ = ip;
    server_port_ = port;

    if (ip == center_server_ip_ && port == center_server_port_)
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("连接中央服务器成功，查询操作服务器IP端口！");

        Protocol::CSCheckServiceInfo check_info;
        check_info.service_type = ServiceType_GAME_OPERATOR;
        this->SendToServer(check_info);
    }
    else
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("连接操作服务器成功，查询游戏列表！");
        UIManager::GetInstance().GetMainView()->SetUIEnable(true);

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
    UIManager::GetInstance().GetMainView()->SetUIEnable(false);

    if (server_ip_ == center_server_ip_ && server_port_ == center_server_port_)
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("与中央服务器断开，开始连接操作服务器！");
        return;
    }

    UIManager::GetInstance().GetMainView()->SetTipsTxt("与操作服务器断开，准备重连！");

//    SLEEP(2000);

    net_mgr_.DelayReconnect();
}

void NetworkAgent::ConnectToServer(IPAddr ip, Port port)
{
    if (!is_connecting_)
    {
        is_connecting_ = true;
        net_mgr_.SyncConnect(ip, port);
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
