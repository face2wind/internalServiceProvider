#include "message_handler.hpp"
#include "cs_protocol_def.hpp"
#include "network_agent.hpp"
#include "operate_manager.hpp"
#include "game_operator_def.hpp"

using namespace Protocol;

MessageHandler::MessageHandler()
{
  op_mgr = new OperateManager();
  
  handler_func_map_["CSGORequestCommandList"] = &MessageHandler::OnRequestCommandList;
  handler_func_map_["CSGORequestCommand"] = &MessageHandler::OnRequestCommand;
}

MessageHandler::~MessageHandler()
{
  delete op_mgr;
}

void MessageHandler::OnRecv(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  if (nullptr == data)
    return;
  
  const std::string msg_type = data->GetTypeName();
  auto func_it_ = handler_func_map_.find(msg_type);
  if (func_it_ != handler_func_map_.end())
    (this->*(func_it_->second))(net_id, data);
  else
    g_debug<<"unknow msg type : "<<msg_type<<std::endl;
}

void MessageHandler::OnRequestCommandList(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  op_mgr->OnRequestCommandList(net_id);
}

void MessageHandler::OnRequestCommand(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  CSGORequestCommand *cmd = (CSGORequestCommand*)data;
  op_mgr->OnRequestCommand(net_id, cmd->project_type, cmd->operate_type);
}


