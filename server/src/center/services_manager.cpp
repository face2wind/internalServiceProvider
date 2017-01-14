#include "services_manager.hpp"

void ServicesManager::OnUpdateServiceInfo(face2wind::NetworkID net_id, int service_type, face2wind::IPAddr ip, face2wind::Port port, bool allow_multiple)
{
  ServiceItem &s_item = service_item_map_[net_id];
  s_item.net_id = net_id;
  s_item.server_type = service_type;
  s_item.ip = ip;
  s_item.port = port;
  s_item.allow_multiple = allow_multiple;
  
}

void ServicesManager::OnCheckServiceInfo(face2wind::NetworkID net_id, int service_type)
{
  std::cout<<"on chesk service info"<<std::endl;
}
