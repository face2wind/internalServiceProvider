#pragma once

#include <elegance.hpp>

struct ServiceItem
{
  ServiceItem() : server_type(), allow_multiple(false), net_id(0), ip(""), port(0) {}
  
  int server_type;
  bool allow_multiple;

  face2wind::NetworkID net_id;
  face2wind::IPAddr ip;
  face2wind::Port port;
};

class ServicesManager
{
 public:
  void OnUpdateServiceInfo(face2wind::NetworkID net_id, int service_type, face2wind::IPAddr ip, face2wind::Port port, bool allow_multiple);
  void OnCheckServiceInfo(face2wind::NetworkID net_id, int service_type);
  
 protected:
  std::map<face2wind::NetworkID, ServiceItem> service_item_map_;
  
};
