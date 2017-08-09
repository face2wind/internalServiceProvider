#include "socket_def.hpp"
#include "socket_accept.hpp"

namespace face2wind {

SocketAccept::SocketAccept() : handler_(nullptr), local_port_(0)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(OnAcceptError(QAbstractSocket::SocketError)));
}

SocketAccept::~SocketAccept()
{
    disconnect(this, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
    disconnect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)),this,SLOT(OnConnectionSocketError(QAbstractSocket::SocketError)));
}

void SocketAccept::ResetHandler(ISocketHandler *handler)
{
    handler_ = handler;
}

bool SocketAccept::Listen(Port port)
{
    local_port_ = port;
    return this->listen(QHostAddress::Any, port);
}

bool SocketAccept::Write(IPAddr ip, Port port, const char *data, int length)
{
    if (nullptr == data || length <= 0)
        return false;

    auto sock_it = endpoint_sock_map_.find(Endpoint(ip, port, local_port_));
    if (sock_it == endpoint_sock_map_.end())
        return false;

    QTcpSocket *cur_sock = sock_it->second;

    QByteArray bytearr;
    //bytearr.append((char*)&length, HEADER_LENGTH);
    bytearr.append(data, length);
    cur_sock->write(bytearr);
    cur_sock->flush();

    return true;
}

bool SocketAccept::Disconnect(IPAddr ip, Port port)
{
    auto sock_it = endpoint_sock_map_.find(Endpoint(ip, port, local_port_));
    if (sock_it == endpoint_sock_map_.end())
        return false;

    QTcpSocket * cur_sock = sock_it->second;
    cur_sock->close();

    return true;
}

bool SocketAccept::CheckOnHandle(IPAddr ip, Port port)
{
    return endpoint_sock_map_.find(Endpoint(ip, port, local_port_)) != endpoint_sock_map_.end();
}

void SocketAccept::OnNewConnection()
{
    QTcpSocket *pSocket = this->nextPendingConnection();
    if (nullptr == pSocket)
        return;

    Endpoint cur_endpoint(pSocket->peerAddress(), pSocket->peerPort(), pSocket->localPort());

    sock_endpoint_map_[pSocket] = cur_endpoint;
    endpoint_sock_map_[cur_endpoint] = pSocket;

    connect(pSocket, SIGNAL(readyRead()), this, SLOT(OnRecv()));
    connect(pSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnect()));
    connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(OnConnectionSocketError(QAbstractSocket::SocketError)));

    if (nullptr != handler_)
        handler_->OnAccept(cur_endpoint.remote_ip_addr, cur_endpoint.remote_port, cur_endpoint.local_port);
      //handler_->OnAccept(pSocket->peerAddress(), pSocket->peerPort(), pSocket->localPort());
}

void SocketAccept::OnRecv()
{
    QTcpSocket *tcp_socket = qobject_cast<QTcpSocket*>(sender());
    if (nullptr == tcp_socket)
        return;

    QByteArray &receive_ba_ = sockcket_receive_ba_map_[tcp_socket];
    receive_ba_.append(tcp_socket->readAll());

    if (nullptr != handler_)
        handler_->OnRecv(tcp_socket->peerAddress(), tcp_socket->peerPort(), tcp_socket->localPort(), receive_ba_.data(), receive_ba_.size());

    receive_ba_.clear();
}

void SocketAccept::OnDisconnect()
{
    QTcpSocket *tcp_socket = qobject_cast<QTcpSocket*>(sender());

    if (nullptr != handler_)
        handler_->OnDisconnect(tcp_socket->peerAddress(), tcp_socket->peerPort(), tcp_socket->localPort());

    disconnect(tcp_socket, SIGNAL(readyRead()), this, SLOT(OnRecv()));
    disconnect(tcp_socket, SIGNAL(disconnected()), this, SLOT(OnDisconnect()));
    disconnect(tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(OnConnectionSocketError(QAbstractSocket::SocketError)));

    tcp_socket->close();

    endpoint_sock_map_.erase(sock_endpoint_map_[tcp_socket]);
    sock_endpoint_map_.erase(tcp_socket);
}

void SocketAccept::OnConnectionSocketError(QAbstractSocket::SocketError)
{
    qDebug() << this->errorString(); //输出错误信息
    this->OnDisconnect();
}

void SocketAccept::OnAcceptError(QAbstractSocket::SocketError)
{
    qDebug() << this->errorString(); //输出错误信息
    this->close();
}

void SocketAccept::OnRecvPackage(QTcpSocket *tcp_socket, char *data, int length)
{
    handler_->OnRecv(tcp_socket->peerAddress(), tcp_socket->peerPort(), tcp_socket->localPort(), data, length);
}

}
