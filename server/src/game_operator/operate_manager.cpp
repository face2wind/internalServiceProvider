#include "operate_manager.hpp"
#include "network/network_agent.hpp"
#include "commondef.hpp"
#include "game_operator_def.hpp"
#include "xml/serverconfig.hpp"

//#include <fstream>

using namespace Protocol;

void HandleCommandThreadTask::Run()
{
  if (nullptr != operate_mgr_)
    operate_mgr_->DoHandleCommand(net_id_, project_type_, operate_type_);
}

OperateManager::OperateManager()
{
  thread_pool_.Run(MAX_HANDLE_THREAD_NUM);
}

OperateManager::~OperateManager()
{
  thread_pool_.Stop();
}

void OperateManager::OnRequestCommandList(face2wind::NetworkID net_id, int project_list_type)
{
  SCGORequestCommandListACK ack;

  const std::vector<int> *p_list = ServerConfig::Instance().GetUserProjectList(project_list_type);
  if (nullptr == p_list)
    return;

  for (int p_type : *p_list)
  {
    const ServerCfgProject *p_cfg = ServerConfig::Instance().GetProjectCfg(p_type);
    if (nullptr == p_cfg)
      continue;

    Protocol::SCGOCommandProjectItem item;
    item.project_type = p_cfg->type;
    item.project_name = p_cfg->name;
    ack.project_list.push_back(item);
  }

  const std::vector<ServerCfgOperate> &op_cfg_list = ServerConfig::Instance().GetOperateList();
  for (ServerCfgOperate o_cfg : op_cfg_list)
  {
    Protocol::SCGOCommandOperateItem item;
    item.operate_type = o_cfg.type;
    item.operate_name = o_cfg.name;
    item.operate_describe = o_cfg.describe;
    ack.operate_list.push_back(item);
  }
  NetworkAgent::GetInstance().SendSerialize(net_id, ack);

  g_debug<<"OperateManager::OnRequestCommandList"<<std::endl; 
}

void OperateManager::OnRequestCommand(face2wind::NetworkID net_id, int project_type, int operate_type)
{
  //do not let the read pipe action block the socket thread, do it in thread pool
  HandleCommandThreadTask *task = new HandleCommandThreadTask(this, net_id, project_type, operate_type);
  if (nullptr != task)
    thread_pool_.AddTask(task);
}

bool OperateManager::CommandLock(const std::string &cmd_str)
{
  on_operating_cmd_mutex_.Lock();
  bool cmd_not_running = (on_operating_cmd_set_.find(cmd_str) == on_operating_cmd_set_.end());
  if (cmd_not_running)
    on_operating_cmd_set_.insert(cmd_str);
  on_operating_cmd_mutex_.Unlock();
  
  return cmd_not_running;
}

bool OperateManager::CommandUnlock(const std::string &cmd_str)
{
  on_operating_cmd_mutex_.Lock();
  on_operating_cmd_set_.erase(cmd_str);
  on_operating_cmd_mutex_.Unlock();

  return true;
}

void OperateManager::DoHandleCommand(face2wind::NetworkID net_id, int project_type, int operate_type)
{
    //g_debug << "OperateManager::OnRequestCommand" << std::endl;
  const ServerCfgProject *pro_cfg = ServerConfig::Instance().GetProjectCfg(project_type);
  const ServerCfgOperate *opt_cfg = ServerConfig::Instance().GetOperateCfg(operate_type);
  if (nullptr == pro_cfg || nullptr == opt_cfg)
  {
    g_debug  <<"project_type or operate_type invalid"<< std::endl;
    return;
  }
  
  std::string cmd_str = std::string("dev_tool.sh ") + pro_cfg->cmd + " " + opt_cfg->cmd;
  
  SCGORequestCommandAck ack;
  ack.project_type = project_type;
  ack.operate_type = operate_type;

  if (!this->CommandLock(cmd_str))
  {
    ack.operate_result = OperateResultType_CANNOT_GET_LOCK;
    NetworkAgent::GetInstance().SendSerialize(net_id, ack);
    return;
  }

  g_debug << cmd_str << std::endl;
      
  //FILE *pp = popen("ls && sleep 3 && ls", "r");
  FILE *pp = popen(cmd_str.c_str(), "r");
  if (!pp)
  {
    g_debug << " popen "<< cmd_str << " Fail"<< std::endl;
    ack.operate_result = OperateResultType_CANNOT_GET_LOCK;
    NetworkAgent::GetInstance().SendSerialize(net_id, ack);
    return;
  }

  SCGOCommandOutput output_msg;
  char tmp[1024];
  while (fgets(tmp, sizeof(tmp), pp) != NULL)
  {
    if (tmp[strlen(tmp) - 1] == '\n')
      tmp[strlen(tmp) - 1] = '\0';
    tmp[strlen(tmp)] = '\0';
    output_msg.output_str_list.clear();
    output_msg.output_str_list.push_back(std::string(tmp));
    NetworkAgent::GetInstance().SendSerialize(net_id, output_msg);  
  }
  pclose(pp);

  ack.operate_result = OperateResultType_SUCC;
  NetworkAgent::GetInstance().SendSerialize(net_id, ack);

  this->CommandUnlock(cmd_str);
}
