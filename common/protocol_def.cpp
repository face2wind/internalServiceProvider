#include "protocol_def.hpp"

namespace Protocol {

__CSRegisterAccountDescribe__ for_describe_register_to___csregisteraccountdescribe__;
__CSLoginDescribe__ for_describe_register_to___cslogindescribe__;
__SCLoginResultDescribe__ for_describe_register_to___scloginresultdescribe__;
__CSRequestFriendListDescribe__ for_describe_register_to___csrequestfriendlistdescribe__;
__UserItemDescribe__ for_describe_register_to___useritemdescribe__;
__SCFriendListDescribe__ for_describe_register_to___scfriendlistdescribe__;
__CSRequestAllUserListDescribe__ for_describe_register_to___csrequestalluserlistdescribe__;
__SCAllUserListDescribe__ for_describe_register_to___scalluserlistdescribe__;
__CSRequestMakeFriendDescribe__ for_describe_register_to___csrequestmakefrienddescribe__;
__SCMakefriendRequestListDescribe__ for_describe_register_to___scmakefriendrequestlistdescribe__;
__CSChatToUserDescribe__ for_describe_register_to___cschattouserdescribe__;
__SCChatToUserDescribe__ for_describe_register_to___scchattouserdescribe__;


void CSRegisterAccount::Serialize(ByteArray &collector) const
{
  collector.WriteString(name);
  collector.WriteString(passwd);
}

void CSRegisterAccount::Unserialize(ByteArray &collector)
{
  name = collector.ReadString();
  passwd = collector.ReadString();
}

void CSLogin::Serialize(ByteArray &collector) const
{
  collector.WriteString(name);
  collector.WriteString(passwd);
}

void CSLogin::Unserialize(ByteArray &collector)
{
  name = collector.ReadString();
  passwd = collector.ReadString();
}

void SCLoginResult::Serialize(ByteArray &collector) const
{
  collector.WriteInt32(result);
}

void SCLoginResult::Unserialize(ByteArray &collector)
{
  result = collector.ReadInt32();
}

void CSRequestFriendList::Serialize(ByteArray &collector) const
{
}

void CSRequestFriendList::Unserialize(ByteArray &collector)
{
}

void UserItem::Serialize(ByteArray &collector) const
{
  collector.WriteUint32(id);
  collector.WriteString(name);
}

void UserItem::Unserialize(ByteArray &collector)
{
  id = collector.ReadUint32();
  name = collector.ReadString();
}

void SCFriendList::Serialize(ByteArray &collector) const
{
  collector.WriteUint16((unsigned short)friend_list.size());
  for (auto array_item : friend_list)
  {
    array_item.Serialize(collector);
  }
}

void SCFriendList::Unserialize(ByteArray &collector)
{
  {
    int array_size = collector.ReadUint16();
    UserItem tmp_attr_value;
    for (int index = 0; index < array_size; ++ index)
    {
      tmp_attr_value.Unserialize(collector);
      friend_list.push_back(tmp_attr_value);
    }
  }
}

void CSRequestAllUserList::Serialize(ByteArray &collector) const
{
}

void CSRequestAllUserList::Unserialize(ByteArray &collector)
{
}

void SCAllUserList::Serialize(ByteArray &collector) const
{
  collector.WriteUint16((unsigned short)user_list.size());
  for (auto array_item : user_list)
  {
    array_item.Serialize(collector);
  }
}

void SCAllUserList::Unserialize(ByteArray &collector)
{
  {
    int array_size = collector.ReadUint16();
    UserItem tmp_attr_value;
    for (int index = 0; index < array_size; ++ index)
    {
      tmp_attr_value.Unserialize(collector);
      user_list.push_back(tmp_attr_value);
    }
  }
}

void CSRequestMakeFriend::Serialize(ByteArray &collector) const
{
  collector.WriteUint32(user_id);
}

void CSRequestMakeFriend::Unserialize(ByteArray &collector)
{
  user_id = collector.ReadUint32();
}

void SCMakefriendRequestList::Serialize(ByteArray &collector) const
{
  collector.WriteUint16((unsigned short)user_list.size());
  for (auto array_item : user_list)
  {
    array_item.Serialize(collector);
  }
}

void SCMakefriendRequestList::Unserialize(ByteArray &collector)
{
  {
    int array_size = collector.ReadUint16();
    UserItem tmp_attr_value;
    for (int index = 0; index < array_size; ++ index)
    {
      tmp_attr_value.Unserialize(collector);
      user_list.push_back(tmp_attr_value);
    }
  }
}

void CSChatToUser::Serialize(ByteArray &collector) const
{
  collector.WriteUint32(user_id);
  collector.WriteString(chat_message);
}

void CSChatToUser::Unserialize(ByteArray &collector)
{
  user_id = collector.ReadUint32();
  chat_message = collector.ReadString();
}

void SCChatToUser::Serialize(ByteArray &collector) const
{
  collector.WriteUint32(sender_user_id);
  collector.WriteUint32(receiver_user_id);
  collector.WriteString(chat_message);
}

void SCChatToUser::Unserialize(ByteArray &collector)
{
  sender_user_id = collector.ReadUint32();
  receiver_user_id = collector.ReadUint32();
  chat_message = collector.ReadString();
}

}

