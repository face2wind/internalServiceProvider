#include "network_agent.hpp"
#include "game_operator_def.hpp"
#include "internal_protocol_def.hpp"
#include "commondef.hpp"

#include <iostream>

using namespace face2wind;

NetworkAgent::NetworkAgent() : center_server_net_id_(0)
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
  net_mgr_.SyncConnect(SERVER_CENTER_IP_ADDR, SERVER_CENTER_LISTEN_PORT);
  net_mgr_.SyncListen(SERVER_GAME_OPERATOR_LISTEN_PORT);
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
  std::cout<<"OnListenFail !"<<std::endl;
}

void NetworkAgent::OnAccept(IPAddr ip, Port port, Port local_port, NetworkID net_id)
{
  std::cout<<"some one connect : " <<ip<<":"<<port<<", netid("<<net_id<<")"<<std::endl;
}

void NetworkAgent::OnConnect(face2wind::IPAddr ip, face2wind::Port port, face2wind::Port local_port, bool success, face2wind::NetworkID net_id)
{
  if (SERVER_CENTER_IP_ADDR == ip && SERVER_CENTER_LISTEN_PORT == port)
  {
    if (success)
    {
      center_server_net_id_ = net_id;
      
      std::cout<<"connect to center server success!"<<std::endl;
      Protocol::RegisterService msg;
      msg.internal_key = "haha_is_me";
      msg.service_type = ServiceType_GAME_OPERATOR;
      msg.server_port = SERVER_GAME_OPERATOR_LISTEN_PORT;
      msg.allow_multiple = 0;
      this->SendSerialize(net_id, msg);
    }
    else
    {
      std::cout<<"connect to center server fail!"<<std::endl;

      Timer::Sleep(1000);
      net_mgr_.SyncConnect(SERVER_CENTER_IP_ADDR, SERVER_CENTER_LISTEN_PORT);
    }
  }
}

void NetworkAgent::OnRecv(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  msg_handler_.OnRecv(net_id, data);
}

void NetworkAgent::OnDisconnect(NetworkID net_id)
{
  std::cout<<"some one disconnect : netid("<<net_id<<")"<<std::endl;

  if (center_server_net_id_ == net_id)
  {
    Timer::Sleep(1000);
    net_mgr_.SyncConnect(SERVER_CENTER_IP_ADDR, SERVER_CENTER_LISTEN_PORT);
  }
}

  
