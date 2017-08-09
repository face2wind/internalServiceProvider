#pragma once

#include "socket_def.hpp"
#include <map>
#include <QTcpServer>
#include <QTcpSocket>

namespace face2wind {

class ISocketHandler;

class SocketAccept : public QTcpServer
{
    Q_OBJECT

 public:
  SocketAccept();
  ~SocketAccept();

  void ResetHandler(ISocketHandler *handler = nullptr);

  bool Listen(Port port);
  bool Write(IPAddr ip, Port port, const char *data, int length);
  bool Disconnect(IPAddr ip, Port port);

  bool CheckOnHandle(IPAddr ip, Port port);

protected slots:
    void OnNewConnection();

    void OnRecv();
    void OnDisconnect();
    void OnConnectionSocketError(QAbstractSocket::SocketError);
    void OnAcceptError(QAbstractSocket::SocketError);


protected:
    void OnRecvPackage(QTcpSocket *pSenderSocket, char *data, int length);

 protected:
  ISocketHandler *handler_;

  Port local_port_;

  std::map<Endpoint, QTcpSocket *> endpoint_sock_map_;
  std::map<QTcpSocket *, Endpoint> sock_endpoint_map_;

  std::map<QTcpSocket *, QByteArray> sockcket_receive_ba_map_;
  //QByteArray receive_ba_;
  int body_length_;
  
};

}
