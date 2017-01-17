#include "cs_protocol_def.hpp"

namespace Protocol {

__CSCheckServiceInfoDescribe__ for_describe_register_to___cscheckserviceinfodescribe__;
__SCCheckServiceInfoAckDescribe__ for_describe_register_to___sccheckserviceinfoackdescribe__;
__CSGORequestCommandListDescribe__ for_describe_register_to___csgorequestcommandlistdescribe__;
__SCGOCommandItemDescribe__ for_describe_register_to___scgocommanditemdescribe__;
__SCGORequestCommandListACKDescribe__ for_describe_register_to___scgorequestcommandlistackdescribe__;
__CSGORequestCommandDescribe__ for_describe_register_to___csgorequestcommanddescribe__;
__SCGORequestCommandAckDescribe__ for_describe_register_to___scgorequestcommandackdescribe__;


void CSCheckServiceInfo::Serialize(ByteArray &collector) const
{
  collector.WriteInt16(service_type);
}

void CSCheckServiceInfo::Unserialize(ByteArray &collector)
{
  service_type = collector.ReadInt16();
}

void SCCheckServiceInfoAck::Serialize(ByteArray &collector) const
{
  collector.WriteInt16(service_type);
  collector.WriteString(ip_addr);
  collector.WriteUint16(port);
}

void SCCheckServiceInfoAck::Unserialize(ByteArray &collector)
{
  service_type = collector.ReadInt16();
  ip_addr = collector.ReadString();
  port = collector.ReadUint16();
}

void CSGORequestCommandList::Serialize(ByteArray &collector) const
{
}

void CSGORequestCommandList::Unserialize(ByteArray &collector)
{
}

void SCGOCommandItem::Serialize(ByteArray &collector) const
{
  collector.WriteInt8(project_type);
  collector.WriteString(project_name);
  collector.WriteInt8(operate_type);
  collector.WriteString(operate_name);
  collector.WriteString(operate_describe);
}

void SCGOCommandItem::Unserialize(ByteArray &collector)
{
  project_type = collector.ReadInt8();
  project_name = collector.ReadString();
  operate_type = collector.ReadInt8();
  operate_name = collector.ReadString();
  operate_describe = collector.ReadString();
}

void SCGORequestCommandListACK::Serialize(ByteArray &collector) const
{
  collector.WriteUint16((unsigned short)command_list.size());
  for (auto array_item : command_list)
  {
    array_item.Serialize(collector);
  }
}

void SCGORequestCommandListACK::Unserialize(ByteArray &collector)
{
  {
    int array_size = collector.ReadUint16();
    SCGOCommandItem tmp_attr_value;
    for (int index = 0; index < array_size; ++ index)
    {
      tmp_attr_value.Unserialize(collector);
      command_list.push_back(tmp_attr_value);
    }
  }
}

void CSGORequestCommand::Serialize(ByteArray &collector) const
{
  collector.WriteInt8(project_type);
  collector.WriteInt8(operate_type);
}

void CSGORequestCommand::Unserialize(ByteArray &collector)
{
  project_type = collector.ReadInt8();
  operate_type = collector.ReadInt8();
}

void SCGORequestCommandAck::Serialize(ByteArray &collector) const
{
  collector.WriteInt8(project_type);
  collector.WriteInt8(operate_type);
  collector.WriteInt32(operate_result);
}

void SCGORequestCommandAck::Unserialize(ByteArray &collector)
{
  project_type = collector.ReadInt8();
  operate_type = collector.ReadInt8();
  operate_result = collector.ReadInt32();
}

}

