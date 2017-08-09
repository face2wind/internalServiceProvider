#pragma once

#include <string>
#include <QHostAddress>

namespace face2wind {

//typedef unsigned short Port;
//typedef std::string IPAddr;
//typedef int NetworkID;

typedef quint16 Port;
typedef QHostAddress IPAddr;
typedef int NetworkID;

static const int MAX_SOCKET_MSG_BUFF_LENGTH = 1024 * 4;
static const int MAX_BACKLOG = 128;

struct Endpoint
{
  Endpoint() : remote_ip_addr(""), remote_port(0), local_port(0) {}
  //Endpoint(IPAddr remote_ip_addr_ = "", Port remote_port_ = 0, Port local_port_ = 0) : remote_ip_addr(remote_ip_addr_), remote_port(remote_port_), local_port(local_port_) {}
  Endpoint(IPAddr remote_ip_addr_, Port remote_port_, Port local_port_) : remote_ip_addr(remote_ip_addr_), remote_port(remote_port_), local_port(local_port_) {}

  bool operator <(const Endpoint &other) const
  {
    if (remote_ip_addr.toString().size() < other.remote_ip_addr.toString().size())
      return true;
    else if (remote_ip_addr.toString().size() > other.remote_ip_addr.toString().size())
      return false;
    else
    {
      if (remote_port < other.remote_port)
        return true;
      else if (remote_port > other.remote_port)
        return false;
      else
        return local_port < other.local_port;
    }
  }

  IPAddr remote_ip_addr;
  Port remote_port;
  Port local_port;
};

class ISocketHandler
{
 public:
  ISocketHandler() {}
  virtual ~ISocketHandler() {}

  virtual void OnAccept(IPAddr remote_ip, Port remote_port, Port local_port) = 0;
  virtual void OnConnect(IPAddr remote_ip, Port remote_port, Port local_port) = 0;
  virtual void OnConnectFail(IPAddr remote_ip, Port remote_port) = 0;
  virtual void OnRecv(IPAddr remote_ip, Port remote_port, Port local_port, char *data, int length) = 0;
  virtual void OnDisconnect(IPAddr remote_ip, Port remote_port, Port local_port) = 0;
};

}
