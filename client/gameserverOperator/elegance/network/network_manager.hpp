#pragma once

#include <set>
#include <map>
#include <stack>

#include <QMutex>
#include <QThread>

//#include "socket_def.hpp"
#include <elegance/network/network_packer_factory.hpp>

namespace face2wind
{

class SocketAccept;
class SocketConnect;
class NetworkManager;

class INetworkHandler
{
 public:
  INetworkHandler() {}
  virtual ~INetworkHandler() {}

  virtual void OnListenFail(Port port) = 0;
  virtual void OnAccept(IPAddr ip, Port port, Port local_port, NetworkID net_id) = 0;
  virtual void OnConnect(IPAddr ip, Port port, Port local_port, bool success, NetworkID net_id) = 0;
  
  virtual void OnRecv(NetworkID net_id, const char *data, int length) = 0;
  virtual void OnDisconnect(NetworkID net_id) = 0;
};

class NetworkManager : public ISocketHandler
{
 public:
  NetworkManager();
  ~NetworkManager();
  
  friend class SocketAccept;
  friend class SocketConnect;
  friend class INetworkPackager;

  void RegistHandler(INetworkHandler *handler);
  void UnregistHandler(INetworkHandler *handler);
  
  void SyncListen(Port port);
  void SyncConnect(IPAddr ip, Port port);
  void SyncConnect(const std::string &ip, Port port);
  
  void Send(NetworkID net_id, const char *data, int length);
  void Disconnect(NetworkID net_id);

  
 protected:
  NetworkID GetFreeNetID();

  virtual void OnAccept(IPAddr remote_ip, Port remote_port, Port local_port);
  virtual void OnConnect(IPAddr remote_ip, Port remote_port, Port local_port);
  virtual void OnConnectFail(IPAddr remote_ip, Port remote_port);
  virtual void OnRecv(IPAddr ip, Port port, Port local_port, char *data, int length);
  virtual void OnDisconnect(IPAddr ip, Port port, Port local_port);

  virtual void SendRaw(NetworkID net_id, const char *data, int length);
  virtual void OnRecvPackage(NetworkID net_id, char *data, int length);

 protected:
  //QSet<INetworkHandler *> handler_list_;
  std::set<INetworkHandler *> handler_list_;
  
  std::set<SocketAccept*> accept_list_;
  std::set<SocketConnect*> connect_list_;

  QMutex connecting_map_lock_;
  std::map<Endpoint, SocketConnect*> connecting_map_;

  QMutex free_net_id_lock_;
  std::stack<NetworkID> free_net_id_list_;
  int max_net_id_;

  QMutex net_id_endpoint_lock_;
  std::map<NetworkID, Endpoint> net_id_to_endpoint_map_;
  std::map<Endpoint, NetworkID> endpoint_to_net_id_map_;

  std::map<NetworkID, SocketAccept*> net_id_to_accept_;
  std::map<NetworkID, SocketConnect*> net_id_to_connect_;

  QMutex accept_list_mutex_;
  QMutex connect_list_mutex_;

  INetworkPackager *packager_;
  NetworkPackerType packer_type_;
};


}

