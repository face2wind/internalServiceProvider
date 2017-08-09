#include "network_manager.hpp"

#include <elegance/platform/network/socket_accept.hpp>
#include <elegance/platform/network/socket_connect.hpp>
#include <elegance/common/debug_message.hpp>

#include <sstream>
#include <QDebug>
#include <iostream>

namespace face2wind
{

NetworkManager::NetworkManager() : max_net_id_(0), connect_list_mutex_(QMutex::Recursive), packager_(nullptr), packer_type_(NetworkPackerType::BEGIN)
{
    packager_ = NetworkPackerFactory::CreatePacker(packer_type_, this);
}

NetworkManager::~NetworkManager()
{
    accept_list_mutex_.lock();
    for (SocketAccept *acceptor : accept_list_)
        delete acceptor;
    accept_list_.clear();
    accept_list_mutex_.unlock();

    connect_list_mutex_.lock();
    for (SocketConnect *connector : connect_list_)
        delete connector;
    connect_list_.clear();
    connect_list_mutex_.unlock();
}

void NetworkManager::RegistHandler(INetworkHandler *handler)
{
    if (handler_list_.find(handler) != handler_list_.end())
        return;

    handler_list_.insert(handler);

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::RegistHandler()" << fDebugEndl;
}

void NetworkManager::UnregistHandler(INetworkHandler *handler)
{
    if (handler_list_.find(handler) == handler_list_.end())
        return;

    //handler_list_.remove(handler);
    handler_list_.erase(handler);

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::UnregistHandler()" << fDebugEndl;
}

void NetworkManager::SyncListen(Port port)
{
    SocketAccept *accept = new SocketAccept();
    accept->ResetHandler(this);

    accept_list_mutex_.lock();
    accept_list_.insert(accept);
    accept_list_mutex_.unlock();

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::SyncListen port = " << port << fDebugEndl;

    bool listen_result = accept->Listen(port);

    if (!listen_result)
    {
        accept_list_mutex_.lock();
        accept_list_.erase(accept);
        accept_list_mutex_.unlock();

        delete accept;

        for (auto handler : handler_list_)
            handler->OnListenFail(port);
    }
}

void NetworkManager::SyncConnect(IPAddr ip, Port port)
{
    SocketConnect *connect = new SocketConnect();
    connect->ResetHandler(this);

    connect_list_mutex_.lock();
    connect_list_.insert(connect);
    connect_list_mutex_.unlock();

    connecting_map_lock_.lock();
    connecting_map_[Endpoint(ip, port, 0)] = connect;
    connecting_map_lock_.unlock();

    bool connect_result = connect->Connect(ip, port);
    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::SyncConnect remote = " << ip.toString().toStdString() << ":" << port << fDebugEndl;

    if (!connect_result)
    {
        this->OnConnectFail(ip, port);
    }
}

void NetworkManager::SyncConnect(const std::string &ip, Port port)
{
    QHostAddress host_addr(QString(ip.c_str()));
    this->SyncConnect(host_addr, port);
}


void NetworkManager::Send(NetworkID net_id, const char *data, int length)
{
    net_id_endpoint_lock_.lock();
    auto endpoint_it = net_id_to_endpoint_map_.find(net_id);
    if (endpoint_it == net_id_to_endpoint_map_.end())
    {
        net_id_endpoint_lock_.unlock();
        return;
    }
    net_id_endpoint_lock_.unlock();

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::Send net_id = " << net_id << " length = " << length << fDebugEndl;

    if (nullptr != packager_)
        packager_->PackAndSend(net_id, data, length);
}

void NetworkManager::Disconnect(NetworkID net_id)
{
    net_id_endpoint_lock_.lock();
    auto endpoint_it = net_id_to_endpoint_map_.find(net_id);
    if (endpoint_it == net_id_to_endpoint_map_.end())
    {
        net_id_endpoint_lock_.unlock();
        return;
    }
    net_id_endpoint_lock_.unlock();

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::Disconnect net_id = " << net_id << fDebugEndl;

    auto accept_it = net_id_to_accept_.find(net_id);
    if (accept_it != net_id_to_accept_.end())
        accept_it->second->Disconnect(endpoint_it->second.remote_ip_addr, endpoint_it->second.remote_port);

    auto connect_it = net_id_to_connect_.find(net_id);
    if (connect_it != net_id_to_connect_.end())
        connect_it->second->Disconnect();
}

NetworkID NetworkManager::GetFreeNetID()
{
    NetworkID tmp_net_id;

    free_net_id_lock_.lock();
    if (!free_net_id_list_.empty())
    {
        tmp_net_id = free_net_id_list_.top();
        free_net_id_list_.pop();
    }
    else
    {
        tmp_net_id = max_net_id_ + 1;
        max_net_id_ = tmp_net_id;
    }
    free_net_id_lock_.unlock();

    return tmp_net_id;
}

// all handle functions

void NetworkManager::OnAccept(IPAddr remote_ip, Port remote_port, Port local_port)
{
    NetworkID net_id = this->GetFreeNetID();

    accept_list_mutex_.lock();
    for (auto accept : accept_list_)
    {
        if (accept->CheckOnHandle(remote_ip, remote_port))
        {
            net_id_to_accept_[net_id] = accept;

            Endpoint end_point(remote_ip, remote_port, local_port);

            net_id_endpoint_lock_.lock();
            net_id_to_endpoint_map_[net_id] = end_point;
            endpoint_to_net_id_map_[end_point] = net_id;
            net_id_endpoint_lock_.unlock();
        }
    }
    accept_list_mutex_.unlock();

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::OnAccept remote = " << remote_ip.toString().toStdString() << ":" << remote_port
                                                   << ", local_port = " << local_port << ", net_id = "<< net_id << fDebugEndl;

    for (INetworkHandler *tmp_handler : handler_list_)
        tmp_handler->OnAccept(remote_ip, remote_port, local_port, net_id);
}

void NetworkManager::OnConnect(IPAddr remote_ip, Port remote_port, Port local_port)
{
    connecting_map_lock_.lock();
    connecting_map_.erase(Endpoint(remote_ip, remote_port, 0));
    connecting_map_lock_.unlock();


    NetworkID net_id = this->GetFreeNetID();

    connect_list_mutex_.lock();
    for (auto connect : connect_list_)
    {
        if (connect->CheckOnHandle(remote_ip, remote_port))
        {
            net_id_to_connect_[net_id] = connect;

            Endpoint end_point(remote_ip, remote_port, local_port);

            net_id_endpoint_lock_.lock();
            net_id_to_endpoint_map_[net_id] = end_point;
            endpoint_to_net_id_map_[end_point] = net_id;
            net_id_endpoint_lock_.unlock();
        }
    }
    connect_list_mutex_.unlock();

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::OnConnect remote = " << remote_ip.toString().toStdString() << ":" << remote_port
                                                   << ", local_port = " << local_port << ", net_id = " << net_id << fDebugEndl;

    for (INetworkHandler *handler : handler_list_)
        handler->OnConnect(remote_ip, remote_port, local_port, true, net_id);
}

void NetworkManager::OnConnectFail(IPAddr remote_ip, Port remote_port)
{
    connecting_map_lock_.lock();
    auto connect_it = connecting_map_.find(Endpoint(remote_ip, remote_port, 0));
    if (connect_it == connecting_map_.end())
    {
        connecting_map_lock_.unlock();
        return;
    }
    SocketConnect *connect = connect_it->second;
    connecting_map_.erase(connect_it);
    connecting_map_lock_.unlock();

    for (auto handler : handler_list_)
        handler->OnConnect(remote_ip, remote_port, connect->GetLocalPort(), false, 0);

    connect_list_mutex_.lock();
    connect_list_.erase(connect);
    connect_list_mutex_.unlock();
    delete connect;
}

void NetworkManager::OnRecv(IPAddr ip, Port port, Port local_port, char *data, int length)
{
    net_id_endpoint_lock_.lock();
    auto net_id_it = endpoint_to_net_id_map_.find(Endpoint(ip, port, local_port));
    if (net_id_it == endpoint_to_net_id_map_.end())
    {
        net_id_endpoint_lock_.unlock();
        return;
    }
    net_id_endpoint_lock_.unlock();

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::OnRecv " << ip.toString().toStdString() << ":"<<port<<", data length = " << length << fDebugEndl;

    if (nullptr != packager_)
        packager_->UnPack(net_id_it->second, data, length);
}

void NetworkManager::OnDisconnect(IPAddr ip, Port port, Port local_port)
{
    net_id_endpoint_lock_.lock();

    auto net_id_it = endpoint_to_net_id_map_.find(Endpoint(ip, port, local_port));
    if (net_id_it == endpoint_to_net_id_map_.end())
    {
        net_id_endpoint_lock_.unlock();
        return;
    }

    NetworkID net_id = net_id_it->second;

    net_id_to_endpoint_map_.erase(net_id);
    endpoint_to_net_id_map_.erase(net_id_it);

    net_id_endpoint_lock_.unlock();

    auto accept_it = net_id_to_accept_.find(net_id);
    if (accept_it != net_id_to_accept_.end())
    {
        net_id_to_accept_.erase(accept_it);
    }

    auto connect_it = net_id_to_connect_.find(net_id);
    if (connect_it != net_id_to_connect_.end())
    {
        SocketConnect *connect = connect_it->second;
        connect_list_.erase(connect);
        net_id_to_connect_.erase(connect_it);

        connect_list_mutex_.lock();
        connect_list_.erase(connect);
        connect_list_mutex_.unlock();
        delete connect;
    }

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::OnDisconnect net_id = " << net_id << fDebugEndl;

    for (auto handler : handler_list_)
        handler->OnDisconnect(net_id);

    free_net_id_list_.push(net_id);
}

void NetworkManager::SendRaw(NetworkID net_id, const char *data, int length)
{
    net_id_endpoint_lock_.lock();

    auto endpoint_it = net_id_to_endpoint_map_.find(net_id);
    if (endpoint_it == net_id_to_endpoint_map_.end())
    {
        net_id_endpoint_lock_.unlock();
        return;
    }
    net_id_endpoint_lock_.unlock();

    auto accept_it = net_id_to_accept_.find(net_id);
    if (accept_it != net_id_to_accept_.end())
        accept_it->second->Write(endpoint_it->second.remote_ip_addr, endpoint_it->second.remote_port, data, length);

    auto connect_it = net_id_to_connect_.find(net_id);
    if (connect_it != net_id_to_connect_.end())
        connect_it->second->Write(data, length);
}

void NetworkManager::OnRecvPackage(NetworkID net_id, char *data, int length)
{
    if (net_id <= 0 || nullptr == data || length <= 0)
        return;

    fDebugWithHead(DebugMessageType::BASE_NETWORK) << "NetworkManager::OnRecvPackage net_id = " << net_id << ", data length = " << length << fDebugEndl;

    for (auto handler : handler_list_)
        handler->OnRecv(net_id, data, length);
}


}
