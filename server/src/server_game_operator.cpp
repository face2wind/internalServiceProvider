#include <iostream>

#include "network/network_agent.hpp"
#include "common/debug_message.hpp"

using namespace std;

int main()
{
  //face2wind::DebugMessage::GetInstance().SetOnshowType(face2wind::DebugMessageType::BASE_NETWORK, true);
  NetworkAgent::GetInstance().Running();
}
