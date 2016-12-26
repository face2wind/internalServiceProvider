#include "msg_handler.h"
#include "protocol_def.hpp"
#include "ui/ui_manager.hpp"
#include <QtCore>

using namespace Protocol;

MessageHandler::MessageHandler()
{
    handler_func_map_["SCLoginResult"] = &MessageHandler::OnLoginResult;
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

void MessageHandler::OnLoginResult(const face2wind::SerializeBase *data)
{
    SCLoginResult *msg = (SCLoginResult*)data;
    qDebug()<<"login result : "<<msg->result;
    UIManager::GetInstance().OnLoginResult(msg->result);
}

void MessageHandler::OnFriendListReturn(const face2wind::SerializeBase *data)
{
    SCFriendList *msg = (SCFriendList*)data;
    qDebug()<<"friend list size"<<msg->friend_list.size();
}

void MessageHandler::OnAllUserListReturn(const face2wind::SerializeBase *data)
{
    SCAllUserList *msg = (SCAllUserList*)data;
}

void MessageHandler::OnReceiveChatMsg(const face2wind::SerializeBase *data)
{
    SCChatToUser *msg = (SCChatToUser*)data;
}
