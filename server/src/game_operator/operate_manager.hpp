#pragma once

#include <vector>
#include <platform/network/socket_def.hpp>
#include "cs_protocol_def.hpp"

class OperateManager
{
 public:
  OperateManager();
  ~OperateManager();
  
  void OnRequestCommandList(face2wind::NetworkID net_id);

 private:
  std::vector<Protocol::SCGOCommandProjectItem> project_list;
  std::vector<Protocol::SCGOCommandOperateItem> operate_list;

};
