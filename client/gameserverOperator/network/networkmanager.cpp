#include "networkmanager.h"
#include <string.h>

NetworkManager::NetworkManager(QObject *parent) : QTcpSocket(parent), body_length_(0), reconnect_timer(this)
{
    connect(this, SIGNAL(connected()), this, SLOT(OnConnect()));
    connect(this, SIGNAL(readyRead()), this, SLOT(OnRecv()));
    connect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnect()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    connect(&reconnect_timer, SIGNAL(timeout()), this, SLOT(OnReconnectTimerTimeOut()));
}

NetworkManager::~NetworkManager()
{
    disconnect(this, SIGNAL(connected()), this, SLOT(OnConnect()));
    disconnect(this, SIGNAL(readyRead()), this, SLOT(OnRecv()));
    disconnect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnect()));
    disconnect(this, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    disconnect(&reconnect_timer, SIGNAL(timeout()), this, SLOT(OnReconnectTimerTimeOut()));
}

NetworkManager & NetworkManager::GetInstance()
{
    static NetworkManager instance;
    return instance;
}

void NetworkManager::RegistHandler(INetworkHandler *handler)
{
    handler_set_.insert(handler);
}

void NetworkManager::UnregistHandler(INetworkHandler *handler)
{
    handler_set_.erase(handler);
}

void NetworkManager::SyncConnect(IPAddr ip, Port port)
{
    this->abort();
    this->connectToHost(ip, port);
}

void NetworkManager::Send(const char *data, int length)
{
    int HEADER_LENGTH = 4;
    QByteArray bytearr;
    bytearr.append((char*)&length, HEADER_LENGTH);
    bytearr.append(data, length);
    this->write(bytearr);
    this->flush();
}

void NetworkManager::Disconnect()
{
    this->abort();
}

void NetworkManager::DelayReconnect()
{
    reconnect_timer.start(2000);
}

// protected:

void NetworkManager::OnConnect()
{
    reconnect_timer.stop();

    QString peer_addr = this->peerAddress().toString();
    qDebug()<<" has connect to "<<peer_addr<<":"<<this->peerPort();

    last_connect_ip = IPAddr(peer_addr);
    last_connect_port = this->peerPort();

    for (INetworkHandler *handler : handler_set_)
    {
        handler->OnConnect(IPAddr(peer_addr), this->peerPort(), this->localPort(), true);
    }
}

void NetworkManager::DoHeadBodyRecv()
{
    receive_ba_.append(this->readAll());
    if (body_length_ <= 0) // hasn't read head
    {
        if (receive_ba_.size() >= NET_PACKER_FIXHEAD_HEADER_LENGTH)
        {
            NetPackerFixHeadMsgHeader *header = (NetPackerFixHeadMsgHeader*)receive_ba_.data();
            body_length_ = header->package_length;
            header = nullptr;

            QByteArray left_data;
            left_data.append(receive_ba_.data() + NET_PACKER_FIXHEAD_HEADER_LENGTH, receive_ba_.size()-NET_PACKER_FIXHEAD_HEADER_LENGTH);
            receive_ba_ = left_data;

            this->DoHeadBodyRecv();
        }
    }
    else // read body
    {
        if (receive_ba_.size() >= body_length_)
        {
            this->OnRecvPackage(receive_ba_.data(), body_length_);

            QByteArray left_data;
            left_data.append(receive_ba_.data() + body_length_, receive_ba_.size() - body_length_);
            receive_ba_ = left_data;

            body_length_ = 0;
            this->DoHeadBodyRecv();
        }
    }
}

void NetworkManager::OnRecv()
{
    //IPAddr ip, Port port, Port local_port, char *data, int length
    this->DoHeadBodyRecv();
}

void NetworkManager::OnDisconnect()
{
    for (INetworkHandler *handler : handler_set_)
    {
        handler->OnDisconnect();
    }
}

void NetworkManager::OnRecvPackage(char *data, int length)
{
    for (INetworkHandler *handler : handler_set_)
    {
        handler->OnRecv(data, length);
    }
}

void NetworkManager::displayError(QAbstractSocket::SocketError)
{
    qDebug() << this->errorString(); //输出错误信息
    this->OnDisconnect();
}

void NetworkManager::OnReconnectTimerTimeOut()
{
    qDebug()<<"reconnect to"<<last_connect_ip.toString()<<":"<<last_connect_port;
    this->SyncConnect(last_connect_ip, last_connect_port);
}
