#include "networkagent.h"
#include "ui/ui_manager.hpp"
#include "protocol/cs_protocol_def.hpp"
#include "commondef.hpp"
#include "ui/mainwindow.h"
#include <QFile>

NetworkAgent::NetworkAgent() : server_ip_(""), server_port_(0), center_server_ip_("127.0.0.1"), center_server_port_(52013), service_type_(ServiceType_GAME_OPERATOR), project_list_type_(0)
{
    net_mgr_.RegistSerializeHandler(this);

    QFile file_project_list("center_server_addr.txt");
    if (file_project_list.open(QFile::ReadOnly))
    {
        QTextStream in(&file_project_list);
        QString ip_str;
        int port;
        int service_type;
        int project_list_type;
        in >> ip_str >> port >> service_type >> project_list_type;
        center_server_ip_ = ip_str;
        center_server_port_ = port;
        service_type_ = service_type;
        project_list_type_ = project_list_type;
    }

    this->ConnectToServer(center_server_ip_, center_server_port_);
}

NetworkAgent & NetworkAgent::GetInstance()
{
    static NetworkAgent instance;
    return instance;
}

void NetworkAgent::OnConnect(IPAddr ip, Port port, Port local_port, bool success, NetworkID net_id)
{
    if (!success)
    {
        return;
    }

    server_ip_ = ip;
    server_port_ = port;
    server_netid_ = net_id;

    if (ip == center_server_ip_ && port == center_server_port_)
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("连接中央服务器成功，查询操作服务器IP端口！");

        Protocol::CSCheckServiceInfo check_info;
        check_info.service_type = service_type_;
        this->SendToServer(check_info);
    }
    else
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("连接操作服务器成功，查询游戏列表！");
        UIManager::GetInstance().GetMainView()->SetUIEnable(true);

        Protocol::CSGORequestCommandList req_command_list;
        req_command_list.project_list_type = project_list_type_;
        this->SendToServer(req_command_list);
    }
}

void NetworkAgent::OnRecv(NetworkID net_id, const face2wind::SerializeBase *data)
{
    msg_handler_.OnRecv(data);
}

void NetworkAgent::OnDisconnect(NetworkID net_id)
{
    UIManager::GetInstance().GetMainView()->SetUIEnable(false);

    if (server_ip_ == center_server_ip_ && server_port_ == center_server_port_)
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("与中央服务器断开，开始连接操作服务器。。。");
        return;
    }

    UIManager::GetInstance().GetMainView()->SetTipsTxt("与操作服务器断开，正在重连。。。。。。");

    server_netid_ = 0;
    //net_mgr_.DelayReconnect();
}

void NetworkAgent::ConnectToServer(IPAddr ip, Port port)
{
    net_mgr_.SyncConnect(ip, port);
}

void NetworkAgent::Disconnect()
{
    net_mgr_.Disconnect(server_netid_);
}

void NetworkAgent::SendToServer(const face2wind::SerializeBase &data)
{
    net_mgr_.SendSerialize(server_netid_, data);
}
