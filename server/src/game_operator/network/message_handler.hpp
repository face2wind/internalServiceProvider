#pragma once

#include <memory/serialize/serialize_manager.hpp>
#include <string>

class MessageHandler;

typedef void (MessageHandler::*MessageHandlerFunc)(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);

class MessageHandler
{
 public:
  MessageHandler();
  ~MessageHandler();
  
  void OnRecv(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);
  
 protected:
  void OnRegisterRequest(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);

 private:
  std::map<std::string, MessageHandlerFunc> handler_func_map_;
  
};