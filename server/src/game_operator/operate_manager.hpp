#pragma once

#include <vector>
#include <set>

#include <platform/network/socket_def.hpp>
#include <platform/thread/mutex.hpp>
#include "cs_protocol_def.hpp"

class OperateManager
{
  struct ProjectItem
  {
    std::string cmd_name;
    std::string show_name;
  };

  struct OperateItem
  {
    std::string cmd_name;
    std::string show_name;
    std::string describe;
  };

 public:
  OperateManager();
  ~OperateManager();
  
  void OnRequestCommandList(face2wind::NetworkID net_id);
  void OnRequestCommand(face2wind::NetworkID net_id, int project_type, int operate_type);

 protected:
  bool CommandLock(const std::string &cmd_str);
  bool CommandUnlock(const std::string &cmd_str);
  
 private:
  std::vector<ProjectItem> project_list_;
  std::vector<OperateItem> operate_list_;

  std::set<std::string> on_operating_cmd_set_;
  face2wind::Mutex on_operating_cmd_mutex_;
};
