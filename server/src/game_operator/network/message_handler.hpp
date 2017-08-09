#pragma once

#include <elegance/memory/serialize/serialize_manager.hpp>
#include <string>

class OperateManager;
class MessageHandler;

typedef void (MessageHandler::*MessageHandlerFunc)(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);

class MessageHandler
{
 public:
  MessageHandler();
  ~MessageHandler();
  
  void OnRecv(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);
  
 protected:
  void OnRequestCommandList(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);
  void OnRequestCommand(face2wind::NetworkID net_id, const face2wind::SerializeBase *data);
  
 private:
  std::map<std::string, MessageHandlerFunc> handler_func_map_;

  OperateManager *op_mgr;
};
