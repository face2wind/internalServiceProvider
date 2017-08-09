#include <iostream>

#include <elegance/common/debug_message.hpp>

#include "network/network_agent.hpp"
#include "xml/serverconfig.hpp"

using namespace std;

int main()
{
  face2wind::DebugMessage::SetOnshowType(face2wind::DebugMessageType::BASE_NETWORK, true);
  if (!ServerConfig::Instance().LoadXml("serverconfig.xml"))
    return -1;

  NetworkAgent::GetInstance().Listening();
  
  return 0;
}
