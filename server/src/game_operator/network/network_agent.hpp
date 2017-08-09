#pragma once

#include "message_handler.hpp"

#include <elegance/elegance.hpp>

class NetworkAgent : public face2wind::ISerializeNetworkHandler
{
public:
  static NetworkAgent & GetInstance();
  
  ~NetworkAgent();
  
  void Running();
  void SendSerialize(face2wind::NetworkID net_id, const face2wind::SerializeBase &data);
  void Disconnect(face2wind::NetworkID net_id);

  virtual void OnListenFail(face2wind::Port port);
  virtual void OnAccept(face2wind::IPAddr ip, face2wind::Port port, face2wind::Port local_port, face2wind::NetworkID net_id);
  virtual void OnConnect(face2wind::IPAddr ip, face2wind::Port port, face2wind::Port local_port, bool success, face2wind::NetworkID net_id);

  virtual void OnRecv(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);
  virtual void OnDisconnect(face2wind::NetworkID net_id);
  
protected:
  NetworkAgent();
  
private:
  face2wind::SerializeNetworkManager net_mgr_;

  MessageHandler msg_handler_;

  std::string center_server_ip_;
  face2wind::Port center_server_port_;
  face2wind::Port game_operator_listen_port_;
  face2wind::NetworkID center_server_net_id_;
};
