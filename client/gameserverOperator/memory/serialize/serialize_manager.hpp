#pragma once

#include "network/networkmanager.h"
#include "serialize_base.hpp"

namespace face2wind {

class ISerializeNetworkHandler : public INetworkHandler
{
 public:
  virtual void OnConnect(IPAddr ip, Port port, Port local_port, bool success) = 0;

  virtual void OnRecv(const char *data, int length) { data ++; length ++; }
  virtual void OnRecv(const SerializeBase *data) = 0;
  virtual void OnDisconnect() = 0;
};

class SerializeNetworkManager : public NetworkManager
{
 public:
  void SendSerialize(const SerializeBase &data);

  void RegistSerializeHandler(ISerializeNetworkHandler *handler);
  void UnregistSerializeHandler(ISerializeNetworkHandler *handler);

 protected:
  virtual void OnRecvPackage(char *data, int length);

 private:
  std::set<ISerializeNetworkHandler *> serialize_handler_list_;
};

}

