#pragma once

#include <string>
#include <vector>
#include <memory/serialize/serialize_base.hpp>

using face2wind::SerializeBase;
using face2wind::SerializeDescribe;
using face2wind::ByteArray;

namespace Protocol {

class CSRegisterAccount : public SerializeBase
{
public:
  std::string name;
  std::string passwd;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "CSRegisterAccount"; }
};

class CSLogin : public SerializeBase
{
public:
  std::string name;
  std::string passwd;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "CSLogin"; }
};

class SCLoginResult : public SerializeBase
{
public:
  int result;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "SCLoginResult"; }
};

class CSRequestFriendList : public SerializeBase
{
public:

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "CSRequestFriendList"; }
};

class UserItem : public SerializeBase
{
public:
  unsigned int id;
  std::string name;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "UserItem"; }
};

class SCFriendList : public SerializeBase
{
public:
  std::vector<UserItem> friend_list;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "SCFriendList"; }
};

class CSRequestAllUserList : public SerializeBase
{
public:

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "CSRequestAllUserList"; }
};

class SCAllUserList : public SerializeBase
{
public:
  std::vector<UserItem> user_list;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "SCAllUserList"; }
};

class CSRequestMakeFriend : public SerializeBase
{
public:
  unsigned int user_id;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "CSRequestMakeFriend"; }
};

class SCMakefriendRequestList : public SerializeBase
{
public:
  std::vector<UserItem> user_list;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "SCMakefriendRequestList"; }
};

class CSChatToUser : public SerializeBase
{
public:
  unsigned int user_id;
  std::string chat_message;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "CSChatToUser"; }
};

class SCChatToUser : public SerializeBase
{
public:
  unsigned int sender_user_id;
  unsigned int receiver_user_id;
  std::string chat_message;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "SCChatToUser"; }
};

class __CSRegisterAccountDescribe__ : public SerializeDescribe
{
public:
  __CSRegisterAccountDescribe__() { GetNameToObjectMap()["CSRegisterAccount"] = this; }
  virtual ~__CSRegisterAccountDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSRegisterAccount(); }
};

class __CSLoginDescribe__ : public SerializeDescribe
{
public:
  __CSLoginDescribe__() { GetNameToObjectMap()["CSLogin"] = this; }
  virtual ~__CSLoginDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSLogin(); }
};

class __SCLoginResultDescribe__ : public SerializeDescribe
{
public:
  __SCLoginResultDescribe__() { GetNameToObjectMap()["SCLoginResult"] = this; }
  virtual ~__SCLoginResultDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCLoginResult(); }
};

class __CSRequestFriendListDescribe__ : public SerializeDescribe
{
public:
  __CSRequestFriendListDescribe__() { GetNameToObjectMap()["CSRequestFriendList"] = this; }
  virtual ~__CSRequestFriendListDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSRequestFriendList(); }
};

class __UserItemDescribe__ : public SerializeDescribe
{
public:
  __UserItemDescribe__() { GetNameToObjectMap()["UserItem"] = this; }
  virtual ~__UserItemDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new UserItem(); }
};

class __SCFriendListDescribe__ : public SerializeDescribe
{
public:
  __SCFriendListDescribe__() { GetNameToObjectMap()["SCFriendList"] = this; }
  virtual ~__SCFriendListDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCFriendList(); }
};

class __CSRequestAllUserListDescribe__ : public SerializeDescribe
{
public:
  __CSRequestAllUserListDescribe__() { GetNameToObjectMap()["CSRequestAllUserList"] = this; }
  virtual ~__CSRequestAllUserListDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSRequestAllUserList(); }
};

class __SCAllUserListDescribe__ : public SerializeDescribe
{
public:
  __SCAllUserListDescribe__() { GetNameToObjectMap()["SCAllUserList"] = this; }
  virtual ~__SCAllUserListDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCAllUserList(); }
};

class __CSRequestMakeFriendDescribe__ : public SerializeDescribe
{
public:
  __CSRequestMakeFriendDescribe__() { GetNameToObjectMap()["CSRequestMakeFriend"] = this; }
  virtual ~__CSRequestMakeFriendDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSRequestMakeFriend(); }
};

class __SCMakefriendRequestListDescribe__ : public SerializeDescribe
{
public:
  __SCMakefriendRequestListDescribe__() { GetNameToObjectMap()["SCMakefriendRequestList"] = this; }
  virtual ~__SCMakefriendRequestListDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCMakefriendRequestList(); }
};

class __CSChatToUserDescribe__ : public SerializeDescribe
{
public:
  __CSChatToUserDescribe__() { GetNameToObjectMap()["CSChatToUser"] = this; }
  virtual ~__CSChatToUserDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSChatToUser(); }
};

class __SCChatToUserDescribe__ : public SerializeDescribe
{
public:
  __SCChatToUserDescribe__() { GetNameToObjectMap()["SCChatToUser"] = this; }
  virtual ~__SCChatToUserDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCChatToUser(); }
};



}

