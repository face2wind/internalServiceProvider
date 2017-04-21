#include "tinyxml2.h"
#include "serverconfig.hpp"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace tinyxml2;
using namespace std;

ServerConfig & ServerConfig::Instance()
{
  static ServerConfig instance;
  return instance;
}

bool ServerConfig::LoadXml(const std::string &xml_path)
{
  XMLDocument* doc = new XMLDocument();
  doc->LoadFile(xml_path.c_str());
  int errorID = doc->ErrorID();
  if (0 != errorID)
  {
    cout << "load xml error = " << errorID << endl;
    return false;
  }

  XMLElement *root_element = doc->FirstChildElement("ServerConfig");
  if (nullptr == root_element)
  {
    cout << "ServerConfig node not exist" << endl;
    return false;
  }

  center_server_ip_ = root_element->FirstChildElement("CenterServer")->FirstChildElement("IpAddr")->GetText();
  center_server_port_ = root_element->FirstChildElement("CenterServer")->FirstChildElement("Port")->IntText();
  game_server_port_ = root_element->FirstChildElement("GameServer")->FirstChildElement("Port")->IntText(0);
  game_server_service_type_ = root_element->FirstChildElement("GameServer")->FirstChildElement("ServiceType")->IntText(0);

  XMLElement *project_element = root_element->FirstChildElement("ProjectList")->FirstChildElement("Project");
  while (nullptr != project_element)
  {
    ServerCfgProject project_cfg;
    project_cfg.type = project_element->IntAttribute("type");
    project_cfg.cmd = project_element->Attribute("cmd");
    project_cfg.name = project_element->Attribute("name");

    if (project_cfg.type != static_cast<int>(project_list_.size()))
    {
      cout << "project type invalid" << endl;
      return false;
    }
    
    project_list_.push_back(project_cfg);
    project_element = project_element->NextSiblingElement();
  }

  XMLElement *operate_element = root_element->FirstChildElement("OperateList")->FirstChildElement("Operate");
  while (nullptr != operate_element)
  {
    ServerCfgOperate operate_cfg;
    operate_cfg.type = operate_element->IntAttribute("type");
    operate_cfg.cmd = operate_element->Attribute("cmd");
    operate_cfg.name = operate_element->Attribute("name");
    operate_cfg.describe = operate_element->Attribute("describe");

    if (operate_cfg.type != static_cast<int>(operate_list_.size()))
    {
      cout << "operate type invalid" << endl;
      return false;
    }

    operate_list_.push_back(operate_cfg);
    operate_element = operate_element->NextSiblingElement();
  }

  XMLElement *pro_list_element = root_element->FirstChildElement("UserProjectList")->FirstChildElement("ListItem");
  while (nullptr != pro_list_element)
  {
    int list_type = pro_list_element->IntAttribute("list_type");
    std::vector<int> &pro_vec = user_project_list_map_[list_type];
    
    XMLElement *pro_type_element = pro_list_element->FirstChildElement("ProjectType");
    while (nullptr != pro_type_element)
    {
      int pro_type = pro_type_element->IntText(0);
      if (pro_type < 0 || pro_type >= static_cast<int>(project_list_.size()))
      {
        cout << "UserProjectList : project type invalid" << endl;
        return false;
      }
      
      pro_vec.push_back(pro_type);
      pro_type_element = pro_type_element->NextSiblingElement();
    }
    
    pro_list_element = pro_list_element->NextSiblingElement();
  }
  
  return true;
}

const ServerCfgProject * ServerConfig::GetProjectCfg(int pro_type)
{
  if (pro_type < 0 || pro_type >= static_cast<int>(project_list_.size()))
    return nullptr;

  return &project_list_[pro_type];
}

const ServerCfgOperate * ServerConfig::GetOperateCfg(int opt_type)
{
  if (opt_type < 0 || opt_type >= static_cast<int>(operate_list_.size()))
    return nullptr;

  return &operate_list_[opt_type];
}

const std::vector<int> * ServerConfig::GetUserProjectList(int project_type)
{
  auto p_list_it = user_project_list_map_.find(project_type);
  if (p_list_it != user_project_list_map_.end())
    return &p_list_it->second;

  return nullptr;
}


