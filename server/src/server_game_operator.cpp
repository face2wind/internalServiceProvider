#include <iostream>

#include "network/network_agent.hpp"
#include "elegance/common/debug_message.hpp"
#include "xml/serverconfig.hpp"

using namespace std;

int main()
{
  //face2wind::DebugMessage::SetOnshowType(face2wind::DebugMessageType::BASE_NETWORK, true);
  if (!ServerConfig::Instance().LoadXml("serverconfig.xml"))
  {
    return -1;
  }
    
  NetworkAgent::GetInstance().Running();
}
