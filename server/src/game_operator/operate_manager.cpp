#include "operate_manager.hpp"
#include "network/network_agent.hpp"
#include "commondef.hpp"
#include "game_operator_def.hpp"

//#include <fiostream.h>
#include <fstream>

using namespace Protocol;

void HandleCommandThreadTask::Run()
{
  if (nullptr != operate_mgr_)
    operate_mgr_->DoHandleCommand(net_id_, project_type_, operate_type_);
}

OperateManager::OperateManager()
{
  std::ifstream file_project_list("project_list.txt", std::ios::in);
  std::ifstream file_operate_list("operate_list.txt", std::ios::in);
  //file_operate_list.open("game_data", std::ios::in);
  
  if (!file_project_list.is_open())
  {
    g_debug << "cannot open file project_list.txt" << std::endl;
    return;
  }
  
  if (!file_operate_list.is_open())
  {
    g_debug << "cannot open file operate_list.txt" << std::endl;
    return;
  }

  while (!file_project_list.eof())
  {
    ProjectItem p_item;
    file_project_list >> p_item.cmd_name >> p_item.show_name;
    if (file_project_list.good())
      project_list_.push_back(p_item);
  }

  while (!file_operate_list.eof())
  {
    OperateItem o_item;
    file_operate_list >> o_item.cmd_name >> o_item.show_name >> o_item.describe;
    if (file_operate_list.good())
      operate_list_.push_back(o_item);
  }
  
  g_debug << "read game_data complete!" << std::endl;
  
  file_operate_list.close();
  file_project_list.close();

  thread_pool_.Run(MAX_HANDLE_THREAD_NUM);
}

OperateManager::~OperateManager()
{
  thread_pool_.Stop();
}

void OperateManager::OnRequestCommandList(face2wind::NetworkID net_id)
{
  SCGORequestCommandListACK ack;

  int project_type(0);
  for (ProjectItem p_item : project_list_)
  {
    Protocol::SCGOCommandProjectItem item;
    item.project_type = project_type;
    item.project_name = p_item.show_name;
    ack.project_list.push_back(item);

    ++ project_type;
  }

  int operate_type(0);
  for (OperateItem o_item : operate_list_)
  {
    Protocol::SCGOCommandOperateItem item;
    item.operate_type = operate_type;
    item.operate_name = o_item.show_name;
    item.operate_describe = o_item.describe;
    ack.operate_list.push_back(item);

    ++ operate_type;
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
  if (project_type >= (int)project_list_.size() || operate_type >= (int)operate_list_.size())
  {
    g_debug  <<"project_type or operate_type invalid"<< std::endl;
    return;
  }

  std::string cmd_str = std::string("dev_tool.sh ") + project_list_[project_type].cmd_name + " " + operate_list_[operate_type].cmd_name;
  
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
