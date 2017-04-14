#pragma once

#include <string>
#include <vector>
#include <memory/serialize/serialize_base.hpp>

using face2wind::SerializeBase;
using face2wind::SerializeDescribe;
using face2wind::ByteArray;

namespace Protocol {

class RegisterService : public SerializeBase
{
public:
  std::string internal_key;
  int service_type;
  unsigned short server_port;
  short allow_multiple;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "RegisterService"; }
};

class RegisterServiceAck : public SerializeBase
{
public:
  std::string name;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);
  virtual const std::string GetTypeName() const { return "RegisterServiceAck"; }
};

class __RegisterServiceDescribe__ : public SerializeDescribe
{
public:
  __RegisterServiceDescribe__() { GetNameToObjectMap()["RegisterService"] = this; }
  virtual ~__RegisterServiceDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new RegisterService(); }
};

class __RegisterServiceAckDescribe__ : public SerializeDescribe
{
public:
  __RegisterServiceAckDescribe__() { GetNameToObjectMap()["RegisterServiceAck"] = this; }
  virtual ~__RegisterServiceAckDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new RegisterServiceAck(); }
};



}

