#include "message_handler.hpp"
#include "cs_protocol_def.hpp"
#include "network_agent.hpp"

using namespace Protocol;

MessageHandler::MessageHandler()
{
  handler_func_map_["CSGORequestCommandList"] = &MessageHandler::OnRequestCommandList;
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

void MessageHandler::OnRequestCommandList(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  SCGORequestCommandListACK ack;

  {
    SCGOCommandProjectItem p_item;
    p_item.project_type = 1;
    p_item.project_name = "Project1";
    ack.project_list.push_back(p_item);
  }

  {
    SCGOCommandOperateItem o_item;
    o_item.operate_type = 1;
    o_item.operate_name = "Operate1";
    o_item.operate_describe = "Test operate 111";
    ack.operate_list.push_back(o_item);
  }
  
  NetworkAgent::GetInstance().SendSerialize(net_id, ack);
}

