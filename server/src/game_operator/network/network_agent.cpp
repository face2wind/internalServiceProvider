#include "network_agent.hpp"
#include "game_operator_def.hpp"
#include "protocol/internal_protocol_def.hpp"
#include "commondef.hpp"
#include <fstream>
#include <iostream>

using namespace face2wind;

NetworkAgent::NetworkAgent() : center_server_port_(0), game_operator_listen_port_(0), center_server_net_id_(0)
{
  net_mgr_.RegistSerializeHandler(this);
}

NetworkAgent::~NetworkAgent()
{

}

NetworkAgent & NetworkAgent::GetInstance()
{
  static NetworkAgent instance;
  return instance;
}

void NetworkAgent::Running()
{
  std::ifstream file_project_list("center_server_addr.txt", std::ios::in);
  bool read_file_succ = false;
  if (file_project_list.is_open())
  {
    file_project_list >> center_server_ip_ >> center_server_port_ >> game_operator_listen_port_;
    if (file_project_list.good()) // read error, set to default ip and port
      read_file_succ = true;
    else
      g_debug<<"read center_server_addr.txt error "<<center_server_ip_ <<":"<<center_server_port_
               <<", "<<game_operator_listen_port_<<std::endl;
  }

  if (!read_file_succ)
  {
      center_server_ip_ = "192.168.11.51";
      center_server_port_ = 52023;
      game_operator_listen_port_ = 52024;
  }

  net_mgr_.SyncConnect(center_server_ip_, center_server_port_);
  net_mgr_.SyncListen(game_operator_listen_port_);
  net_mgr_.WaitAllThread();
}

void NetworkAgent::SendSerialize(face2wind::NetworkID net_id, const face2wind::SerializeBase &data)
{
  net_mgr_.SendSerialize(net_id, data);
}

void NetworkAgent::Disconnect(face2wind::NetworkID net_id)
{
  net_mgr_.Disconnect(net_id);
}

void NetworkAgent::OnListenFail(Port port)
{
  g_debug<<"OnListenFail !"<<std::endl;
}

void NetworkAgent::OnAccept(IPAddr ip, Port port, Port local_port, NetworkID net_id)
{
  g_debug<<"some one connect : " <<ip<<":"<<port<<", netid("<<net_id<<")"<<std::endl;
}

void NetworkAgent::OnConnect(face2wind::IPAddr ip, face2wind::Port port, face2wind::Port local_port, bool success, face2wind::NetworkID net_id)
{
  if (center_server_ip_ == ip && center_server_port_ == port)
  {
    if (success)
    {
      center_server_net_id_ = net_id;
      
      g_debug<<"connect to center server success!"<<std::endl;
      Protocol::RegisterService msg;
      msg.internal_key = "haha_is_me";
      msg.service_type = ServiceType_GAME_OPERATOR;
      msg.server_port = game_operator_listen_port_;
      msg.allow_multiple = 0;
      this->SendSerialize(net_id, msg);
    }
    else
    {
      g_debug<<"connect to center server fail!"<<std::endl;

      Timer::Sleep(1000);
      net_mgr_.SyncConnect(center_server_ip_, center_server_port_);
    }
  }
}

void NetworkAgent::OnRecv(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  msg_handler_.OnRecv(net_id, data);
}

void NetworkAgent::OnDisconnect(NetworkID net_id)
{
  g_debug<<"some one disconnect : netid("<<net_id<<")"<<std::endl;

  if (center_server_net_id_ == net_id)
  {
    Timer::Sleep(1000);
    net_mgr_.SyncConnect(center_server_ip_, center_server_port_);
  }
}

  
