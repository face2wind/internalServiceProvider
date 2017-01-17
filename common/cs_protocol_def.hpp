#pragma once

#include <string>
#include <vector>
#include <memory/serialize/serialize_base.hpp>

using face2wind::SerializeBase;
using face2wind::SerializeDescribe;
using face2wind::ByteArray;

namespace Protocol {

class CSCheckServiceInfo : public SerializeBase
{
public:
  short service_type;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "CSCheckServiceInfo"; }
};

class SCCheckServiceInfoAck : public SerializeBase
{
public:
  short service_type;
  std::string ip_addr;
  unsigned short port;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "SCCheckServiceInfoAck"; }
};

class CSGORequestCommandList : public SerializeBase
{
public:

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "CSGORequestCommandList"; }
};

class SCGOCommandItem : public SerializeBase
{
public:
  char project_type;
  std::string onshow_name;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "SCGOCommandItem"; }
};

class SCGORequestCommandListACK : public SerializeBase
{
public:
  std::vector<SCGOCommandItem> command_item;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "SCGORequestCommandListACK"; }
};

class CSGORequestCommand : public SerializeBase
{
public:
  char project_type;
  char operate_type;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "CSGORequestCommand"; }
};

class SCGORequestCommandAck : public SerializeBase
{
public:
  char project_type;
  char operate_type;
  int operate_result;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "SCGORequestCommandAck"; }
};

class __CSCheckServiceInfoDescribe__ : public SerializeDescribe
{
public:
  __CSCheckServiceInfoDescribe__() { GetNameToObjectMap()["CSCheckServiceInfo"] = this; }
  virtual ~__CSCheckServiceInfoDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSCheckServiceInfo(); }
};

class __SCCheckServiceInfoAckDescribe__ : public SerializeDescribe
{
public:
  __SCCheckServiceInfoAckDescribe__() { GetNameToObjectMap()["SCCheckServiceInfoAck"] = this; }
  virtual ~__SCCheckServiceInfoAckDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCCheckServiceInfoAck(); }
};

class __CSGORequestCommandListDescribe__ : public SerializeDescribe
{
public:
  __CSGORequestCommandListDescribe__() { GetNameToObjectMap()["CSGORequestCommandList"] = this; }
  virtual ~__CSGORequestCommandListDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSGORequestCommandList(); }
};

class __SCGOCommandItemDescribe__ : public SerializeDescribe
{
public:
  __SCGOCommandItemDescribe__() { GetNameToObjectMap()["SCGOCommandItem"] = this; }
  virtual ~__SCGOCommandItemDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCGOCommandItem(); }
};

class __SCGORequestCommandListACKDescribe__ : public SerializeDescribe
{
public:
  __SCGORequestCommandListACKDescribe__() { GetNameToObjectMap()["SCGORequestCommandListACK"] = this; }
  virtual ~__SCGORequestCommandListACKDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCGORequestCommandListACK(); }
};

class __CSGORequestCommandDescribe__ : public SerializeDescribe
{
public:
  __CSGORequestCommandDescribe__() { GetNameToObjectMap()["CSGORequestCommand"] = this; }
  virtual ~__CSGORequestCommandDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSGORequestCommand(); }
};

class __SCGORequestCommandAckDescribe__ : public SerializeDescribe
{
public:
  __SCGORequestCommandAckDescribe__() { GetNameToObjectMap()["SCGORequestCommandAck"] = this; }
  virtual ~__SCGORequestCommandAckDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCGORequestCommandAck(); }
};



}

