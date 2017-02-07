#include "operate_manager.hpp"
#include "network/network_agent.hpp"

using namespace Protocol;

OperateManager::OperateManager()
{
  
  /*
  {
    SCGOCommandProjectItem p_item;
    p_item.project_type = 1;
    p_item.project_name = "ttaxx_cn";
    ack.project_list.push_back(p_item);
  }

  {
    SCGOCommandOperateItem o_item;
    o_item.operate_type = 1;
    o_item.operate_name = "update";
    o_item.operate_describe = "update config and restart server";
    ack.operate_list.push_back(o_item);
  }
  */
  
}

OperateManager::~OperateManager()
{

}

void OperateManager::OnRequestCommandList(face2wind::NetworkID net_id)
{
  std::cout<<"MessageHandler::OnRequestCommandList"<<std::endl; 

  SCGORequestCommandListACK ack;
  ack.project_list = project_list;
  ack.operate_list = operate_list;
  NetworkAgent::GetInstance().SendSerialize(net_id, ack);
}


