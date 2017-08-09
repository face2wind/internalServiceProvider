#include "socket_connect.hpp"
#include <elegance/network/network_packer/network_packer_fixhead.hpp>

namespace face2wind {

SocketConnect::SocketConnect() : handler_(nullptr), running_(false), remote_port_(0), local_port_(0)
{
    connect(this, SIGNAL(connected()), this, SLOT(OnConnect()));
    connect(this, SIGNAL(readyRead()), this, SLOT(OnRecv()));
    connect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnect()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    connect(&reconnect_timer, SIGNAL(timeout()), this, SLOT(OnReconnectTimerTimeOut()));
}

SocketConnect::~SocketConnect()
{
    disconnect(this, SIGNAL(connected()), this, SLOT(OnConnect()));
    disconnect(this, SIGNAL(readyRead()), this, SLOT(OnRecv()));
    disconnect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnect()));
    disconnect(this, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    disconnect(&reconnect_timer, SIGNAL(timeout()), this, SLOT(OnReconnectTimerTimeOut()));
}

void SocketConnect::ResetHandler(ISocketHandler *handler)
{
  handler_ = handler;
}

bool SocketConnect::Disconnect()
{
  running_ = false;
  return true;
}

bool SocketConnect::Connect(IPAddr ip, Port port)
{
  if (running_)
    return false;

  remote_ip_addr_ = ip;
  remote_port_ = port;

  this->abort();
  this->connectToHost(ip, port);

  running_ = false;
  return true;
}

bool SocketConnect::Write(const char *data, int length)
{
    QByteArray bytearr;
    bytearr.append(data, length);
    this->write(bytearr);
    this->flush();

    return true;
}

bool SocketConnect::CheckOnHandle(IPAddr ip, Port port)
{
  return (ip == remote_ip_addr_ && port == remote_port_);
}

void SocketConnect::OnConnect()
{
    reconnect_timer.stop();

    QString peer_addr = this->peerAddress().toString();

    remote_ip_addr_ = IPAddr(peer_addr);
    remote_port_ = this->peerPort();

//    if (nullptr != handler_)
//        handler_->OnConnect(IPAddr(peer_addr), this->peerPort(), this->localPort());
    if (nullptr != handler_)
        handler_->OnConnect(remote_ip_addr_, remote_port_, this->localPort());
}

void SocketConnect::OnRecv()
{
    receive_ba_.append(this->readAll());

    if (nullptr != handler_)
        handler_->OnRecv(this->peerAddress(), this->peerPort(), this->localPort(), receive_ba_.data(), receive_ba_.size());

    receive_ba_.clear();
    //this->DoHeadBodyRecv();
}

void SocketConnect::OnDisconnect()
{
    handler_->OnDisconnect(remote_ip_addr_, remote_port_, local_port_);
}

void SocketConnect::OnRecvPackage(char *data, int length)
{
    handler_->OnRecv(remote_ip_addr_, remote_port_, local_port_, data, length);
}

void SocketConnect::displayError(QAbstractSocket::SocketError error)
{
    qDebug() << "Error("<<this->error()<<"), "<<this->errorString(); //输出错误信息

    switch(error)
    {
    case QAbstractSocket::ConnectionRefusedError:
    case QAbstractSocket::HostNotFoundError:
        handler_->OnConnectFail(remote_ip_addr_, remote_port_);
        break;

    default:
        this->OnDisconnect();
        break;
    }
}

void SocketConnect::OnReconnectTimerTimeOut()
{
    qDebug()<<"reconnect to"<<remote_ip_addr_.toString()<<":"<<remote_port_;
    this->Connect(remote_ip_addr_, remote_port_);
}

}
