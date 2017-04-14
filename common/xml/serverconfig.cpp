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

  XMLElement *project_element = root_element->FirstChildElement("ProjectList")->FirstChildElement("Project");
  while (nullptr != project_element)
  {
    ServerCfgProject project_cfg;
    project_cfg.type = project_element->IntAttribute("type");
    project_cfg.cmd = project_element->Attribute("cmd");
    project_cfg.name = project_element->Attribute("name");
    project_list_.push_back(project_cfg);
    
    project_element = project_element->NextSiblingElement();
  }
  
  return true;
}


//cout <<center_server_element->FirstChildElement("addr")->Attribute("ip")<<endl;

