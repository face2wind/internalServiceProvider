#pragma once

#include "socket_def.hpp"
#include <QTcpSocket>
#include <QTimer>

namespace face2wind {

class ISocketHandler;

class SocketConnect : public QTcpSocket
{
    Q_OBJECT

 public:
  SocketConnect();
  ~SocketConnect();
  
  void ResetHandler(ISocketHandler *handler = nullptr);

  bool Connect(IPAddr ip, Port port);
  bool Write(const char *data, int length);
  bool Disconnect();

  bool CheckOnHandle(IPAddr ip, Port port);
  Port GetLocalPort() { return local_port_;  }

protected slots:
  virtual void OnConnect();
  void OnRecv();
  virtual void OnDisconnect();

  virtual void OnRecvPackage(char *data, int length);
  virtual void displayError(QAbstractSocket::SocketError error);  //显示错误
  void OnReconnectTimerTimeOut();

 protected:
  ISocketHandler *handler_;

  bool running_;

  IPAddr remote_ip_addr_;
  Port remote_port_;
  Port local_port_;

  QTimer reconnect_timer;
  QByteArray receive_ba_;
  int body_length_;

};

}
