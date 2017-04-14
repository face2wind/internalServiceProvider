#include "services_manager.hpp"
#include "protocol/cs_protocol_def.hpp"
#include "network/network_agent.hpp"
#include "center_def.hpp"
#include "services_manager.hpp"

void ServicesManager::OnUpdateServiceInfo(face2wind::NetworkID net_id, int service_type, face2wind::IPAddr ip, face2wind::Port port, bool allow_multiple)
{
  ServiceItem &s_item = service_item_map_[net_id];
  s_item.net_id = net_id;
  s_item.server_type = service_type;
  s_item.ip = ip;
  s_item.port = port;
  s_item.allow_multiple = allow_multiple;
  c_debug<<"update service "<<net_id<<", type "<<service_type<<", ip "<<ip<<", port "<<port<<std::endl;
}

void ServicesManager::OnCheckServiceInfo(face2wind::NetworkID net_id, int service_type)
{
  c_debug <<"on chesk service info"<<std::endl;
  Protocol::SCCheckServiceInfoAck ack;
  ack.service_type = service_type;
  ack.ip_addr = "";
  ack.port = 0;
  
  for (auto service_item : service_item_map_)
  {
    if (service_type == service_item.second.server_type)
    {
      ack.service_type = service_item.second.server_type;
      ack.ip_addr = service_item.second.ip;
      ack.port = service_item.second.port;
      c_debug << "send Protocol::SCCheckServiceInfoAck - service_type:"<<ack.service_type<<", ip:"<<ack.ip_addr<<",ack.port:"<<ack.port<<std::endl;
      break;
    }
  }
  NetworkAgent::GetInstance().SendSerialize(net_id, ack);
}

void ServicesManager::OnDisconnect(face2wind::NetworkID net_id)
{
  service_item_map_.erase(net_id);
}
