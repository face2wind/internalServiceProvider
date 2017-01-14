#pragma once

#include <memory/serialize/serialize_manager.hpp>
#include <string>

class MessageHandler;
class ServicesManager;

typedef void (MessageHandler::*MessageHandlerFunc)(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);

class MessageHandler
{
 public:
  MessageHandler();
  ~MessageHandler();
  
  void OnRecv(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);
  void UpdateNetMsg(face2wind::NetworkID net_id, face2wind::IPAddr ip);
  
 protected:
  void OnUpdateServiceInfoRequest(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);
  void OnCheckServiceInfo(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);
  
  face2wind::IPAddr GetIpWithNetID(face2wind::NetworkID net_id);
  
 private:
  std::map<std::string, MessageHandlerFunc> handler_func_map_;

  ServicesManager *services_mgr_;
  std::map<face2wind::NetworkID, face2wind::IPAddr> net_id_2_ip_map_;
};
