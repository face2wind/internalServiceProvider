#include <iostream>

#include "network/network_agent.hpp"
#include "common/debug_message.hpp"
#include "xml/serverconfig.hpp"

using namespace std;

int main()
{
  //face2wind::DebugMessage::GetInstance().SetOnshowType(face2wind::DebugMessageType::BASE_NETWORK, true);
  ServerConfig::Instance().LoadXml("../conf/serverconfig.xml");
  NetworkAgent::GetInstance().Running();
}
