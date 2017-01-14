#include "message_handler.hpp"
#include "cs_protocol_def.hpp"

using namespace Protocol;

MessageHandler::MessageHandler()
{
  handler_func_map_["CSRegisterAccount"] = &MessageHandler::OnRegisterRequest;
}

MessageHandler::~MessageHandler()
{

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
    std::cout<<"unknow msg type : "<<msg_type<<std::endl;
}

void MessageHandler::OnRegisterRequest(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  //CSRegisterAccount *reg_msg = (CSRegisterAccount*)data;
  //std::cout<<"receive register req : name("<<reg_msg->name<<") password ("<<reg_msg->passwd<<")"<<std::endl;
  //PlayerManager::GetInstance().OnRegisterPlayer(net_id, reg_msg->name, reg_msg->passwd);
}

