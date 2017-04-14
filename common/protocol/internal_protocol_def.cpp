#include "internal_protocol_def.hpp"

namespace Protocol {

__RegisterServiceDescribe__ for_describe_register_to___registerservicedescribe__;
__RegisterServiceAckDescribe__ for_describe_register_to___registerserviceackdescribe__;


void RegisterService::Serialize(ByteArray &collector) const
{
  collector.WriteString(internal_key);
  collector.WriteInt32(service_type);
  collector.WriteUint16(server_port);
  collector.WriteInt16(allow_multiple);
}

void RegisterService::Unserialize(ByteArray &collector)
{
  internal_key = collector.ReadString();
  service_type = collector.ReadInt32();
  server_port = collector.ReadUint16();
  allow_multiple = collector.ReadInt16();
}

void RegisterServiceAck::Serialize(ByteArray &collector) const
{
  collector.WriteString(name);
}

void RegisterServiceAck::Unserialize(ByteArray &collector)
{
  name = collector.ReadString();
}

}

