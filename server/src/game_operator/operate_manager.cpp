#include "operate_manager.hpp"
#include "network/network_agent.hpp"

#include <iostream>
//#include <fiostream.h>
#include <fstream>

using namespace Protocol;
using std::cout;
using std::endl;

OperateManager::OperateManager()
{
  std::ifstream file_project_list("project_list.txt", std::ios::in);
  std::ifstream file_operate_list("operate_list.txt", std::ios::in);
  //file_operate_list.open("game_data", std::ios::in);
  
  if (!file_project_list.is_open())
  {
    cout << "cannot open file project_list.txt" << endl;
    return;
  }
  
  if (!file_operate_list.is_open())
  {
    cout << "cannot open file operate_list.txt" << endl;
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
  
  cout << "read game_data complete!" << endl;
  
  file_operate_list.close();
  file_project_list.close();
}

OperateManager::~OperateManager()
{

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

  std::cout<<"OperateManager::OnRequestCommandList"<<std::endl; 
}

void OperateManager::OnRequestCommand(face2wind::NetworkID net_id, int project_type, int operate_type)
{
  //cout << "OperateManager::OnRequestCommand" << endl;
  if (project_type >= (int)project_list_.size() || operate_type >= (int)operate_list_.size())
  {
    cout <<"project_type or operate_type invalid"<< endl;
    return;
  }
  
  std::string cmd_str = std::string("dev_tool.sh ") + project_list_[project_type].cmd_name + " " + operate_list_[operate_type].cmd_name;
  cout << cmd_str << endl;

  cmd_str = "ls";
  FILE *pp = popen(cmd_str.c_str(), "r");
  if (!pp)
    return;

  char tmp[1024]; //设置一个合适的长度，以存储每一行输出
  while (fgets(tmp, sizeof(tmp), pp) != NULL)
  {
    if (tmp[strlen(tmp) - 1] == '\n') {
      tmp[strlen(tmp) - 1] = '\0'; //去除换行符
    }
    cout << tmp << endl;
    //resvec.push_back(tmp);
  }
  pclose(pp);
}

