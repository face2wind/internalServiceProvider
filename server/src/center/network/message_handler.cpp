#include "message_handler.hpp"
#include "protocol/cs_protocol_def.hpp"
#include "protocol/internal_protocol_def.hpp"
#include "services_manager.hpp"
#include "center_def.hpp"

using namespace Protocol;

MessageHandler::MessageHandler()
{
  services_mgr_ = new ServicesManager();
  
  handler_func_map_["RegisterService"] = &MessageHandler::OnUpdateServiceInfoRequest;
  handler_func_map_["CSCheckServiceInfo"] = &MessageHandler::OnCheckServiceInfo;
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
    c_debug<<"unknow msg type : "<<msg_type<<std::endl;
}

void MessageHandler::UpdateNetMsg(face2wind::NetworkID net_id, face2wind::IPAddr ip)
{
  net_id_2_ip_map_[net_id] = ip;
}

void MessageHandler::OnUpdateServiceInfoRequest(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  RegisterService *reg_msg = (RegisterService*)data;
  c_debug<<"receive register req : key("<<reg_msg->internal_key<<") service_type ("<<reg_msg->service_type
           <<") server_port ("<<reg_msg->server_port<<") allow_multiple ("<<reg_msg->allow_multiple<<std::endl;
  services_mgr_->OnUpdateServiceInfo(net_id, reg_msg->service_type, this->GetIpWithNetID(net_id), reg_msg->server_port, reg_msg->allow_multiple);
}

void MessageHandler::OnCheckServiceInfo(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  CSCheckServiceInfo *check_msg = (CSCheckServiceInfo*)data;
  services_mgr_->OnCheckServiceInfo(net_id, check_msg->service_type);
}

face2wind::IPAddr MessageHandler::GetIpWithNetID(face2wind::NetworkID net_id)
{
  auto ip_it = net_id_2_ip_map_.find(net_id);
  if (ip_it != net_id_2_ip_map_.end())
    return ip_it->second;

  return "";
}
