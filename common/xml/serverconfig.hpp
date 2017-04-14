#pragma once

#include <string>
#include <vector>
#include <map>

struct ServerCfgProject
{
  ServerCfgProject() : type(-1) {}
  
  int type;
  std::string name;
  std::string cmd;
};

struct ServerCfgOperate
{
  ServerCfgOperate() : type(-1) {}
  
  int type;
  std::string name;
  std::string cmd;
  std::string detail;
};

class ServerConfig
{
 public:
  static ServerConfig & Instance();

  bool LoadXml(const std::string &xml_path);

 protected:
  ServerConfig() {}
  ~ServerConfig() {}
  
 private:
  std::string center_server_ip_;
  int center_server_port_;
  int game_server_port_;

  std::vector<ServerCfgProject> project_list_;
  std::vector<ServerCfgOperate> operate_list_;
  std::map<int, std::vector<int> > user_project_list_map_;
};
