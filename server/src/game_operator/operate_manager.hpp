#pragma once

#include <vector>
#include <set>

#include <platform/network/socket_def.hpp>
#include <platform/thread/mutex.hpp>
#include <platform/thread/thread_pool.hpp>
#include "protocol/cs_protocol_def.hpp"

class OperateManager;

class HandleCommandThreadTask : public face2wind::ThreadPoolWorkingTask
{
 public:
  HandleCommandThreadTask(OperateManager *operate_mgr, face2wind::NetworkID net_id, int project_type, int operate_type) :
      operate_mgr_(operate_mgr), net_id_(net_id), project_type_(project_type), operate_type_(operate_type) {}
  
  virtual void Run();

 private:
  OperateManager *operate_mgr_;
  face2wind::NetworkID net_id_;
  int project_type_;
  int operate_type_;
};

class OperateManager
{
  const static int MAX_HANDLE_THREAD_NUM = 8;

 public:
  OperateManager();
  ~OperateManager();

  friend class HandleCommandThreadTask;
  
  void OnRequestCommandList(face2wind::NetworkID net_id);
  void OnRequestCommand(face2wind::NetworkID net_id, int project_type, int operate_type);

 protected:
  bool CommandLock(const std::string &cmd_str);
  bool CommandUnlock(const std::string &cmd_str);
  void DoHandleCommand(face2wind::NetworkID net_id, int project_type, int operate_type);
  
 private:
  face2wind::ThreadPool thread_pool_;
  
  std::set<std::string> on_operating_cmd_set_;
  face2wind::Mutex on_operating_cmd_mutex_;
};
