#ifndef MSG_HANDLER_HPP
#define MSG_HANDLER_HPP

#include <map>
#include <memory/serialize/serialize_base.hpp>

class MessageHandler;
typedef void (MessageHandler::*MessageHandlerFunc)(const face2wind::SerializeBase *data);

class MessageHandler
{
public:
    MessageHandler();
    ~MessageHandler();

    void OnRecv(const face2wind::SerializeBase *data);

protected:
    void CheckServiceInfoAck(const face2wind::SerializeBase *data);
    void OnFriendListReturn(const face2wind::SerializeBase *data);
    void OnAllUserListReturn(const face2wind::SerializeBase *data);
    void OnReceiveChatMsg(const face2wind::SerializeBase *data);

private:
    std::map<std::string, MessageHandlerFunc> handler_func_map_;
};

#endif // NETWORKAGENT_H
