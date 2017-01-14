#include "network_agent.hpp"

#include <iostream>

using namespace face2wind;

NetworkAgent::NetworkAgent()
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

void NetworkAgent::Listening()
{
  net_mgr_.SyncListen(52013);
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
  msg_handler_.UpdateNetMsg(net_id, ip);
}

void NetworkAgent::OnConnect(face2wind::IPAddr ip, face2wind::Port port, face2wind::Port local_port, bool success, face2wind::NetworkID net_id)
{

}

void NetworkAgent::OnRecv(face2wind::NetworkID net_id, const face2wind::SerializeBase *data)
{
  msg_handler_.OnRecv(net_id, data);
}

void NetworkAgent::OnDisconnect(NetworkID net_id)
{
  std::cout<<"some one disconnect : netid("<<net_id<<")"<<std::endl;
  //PlayerManager::GetInstance().OnClientDisconnect(net_id);
}

  