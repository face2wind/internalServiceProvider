#include "msg_handler.h"
#include "cs_protocol_def.hpp"
#include "ui/ui_manager.hpp"
#include "networkagent.h"
#include <QtCore>

using namespace Protocol;

MessageHandler::MessageHandler()
{
    handler_func_map_["SCCheckServiceInfoAck"] = &MessageHandler::CheckServiceInfoAck;
    handler_func_map_["SCFriendList"] = &MessageHandler::OnFriendListReturn;
    handler_func_map_["SCAllUserList"] = &MessageHandler::OnAllUserListReturn;
    handler_func_map_["SCChatToUser"] = &MessageHandler::OnReceiveChatMsg;
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::OnRecv(const face2wind::SerializeBase *data)
{
    auto func_it_ = handler_func_map_.find(data->GetTypeName());
    if (func_it_ == handler_func_map_.end())
        qDebug()<<"unknow msg type : "<<data->GetTypeName().c_str();
    else
        (this->*(func_it_->second))(data);
}

void MessageHandler::CheckServiceInfoAck(const face2wind::SerializeBase *data)
{
    NetworkAgent::GetInstance().Disconnect();

    Protocol::SCCheckServiceInfoAck *ack = (Protocol::SCCheckServiceInfoAck*)data;
    NetworkAgent::GetInstance().ConnectToServer(IPAddr(ack->ip_addr.c_str()), ack->port);
}

void MessageHandler::OnFriendListReturn(const face2wind::SerializeBase *data)
{
}

void MessageHandler::OnAllUserListReturn(const face2wind::SerializeBase *data)
{
}

void MessageHandler::OnReceiveChatMsg(const face2wind::SerializeBase *data)
{
}
