#include "msg_handler.h"
#include "cs_protocol_def.hpp"
#include "ui/ui_manager.hpp"
#include "networkagent.h"
#include <QtCore>
#include "ui/mainwindow.h"
#include "commondef.hpp"

using namespace Protocol;

MessageHandler::MessageHandler()
{
    handler_func_map_["SCCheckServiceInfoAck"] = &MessageHandler::OnCheckServiceInfoAck;
    handler_func_map_["SCGORequestCommandListACK"] = &MessageHandler::OnRequestCommandListACK;
    handler_func_map_["SCGOCommandOutput"] = &MessageHandler::OnCommandOutput;
    handler_func_map_["SCGORequestCommandAck"] = &MessageHandler::OnCommandAck;
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

void MessageHandler::OnCheckServiceInfoAck(const face2wind::SerializeBase *data)
{
    NetworkAgent::GetInstance().Disconnect();

    Protocol::SCCheckServiceInfoAck *ack = (Protocol::SCCheckServiceInfoAck*)data;
    if (0 == ack->port)
    {
        qDebug()<<"MessageHandler::OnCheckServiceInfoAck receive port == 0";
        UIManager::GetInstance().GetMainView()->SetTipsTxt("游戏操作服务器未启动，请联系服务端！");
        return;
    }
    NetworkAgent::GetInstance().ConnectToServer(IPAddr(ack->ip_addr.c_str()), ack->port);
}

void MessageHandler::OnRequestCommandListACK(const face2wind::SerializeBase *data)
{
    Protocol::SCGORequestCommandListACK *ack = (Protocol::SCGORequestCommandListACK*)data;
    UIManager::GetInstance().GetMainView()->SetOperateData(ack);

    UIManager::GetInstance().GetMainView()->SetTipsTxt("游戏列表信息正确获取，请选择操作！");
}

void MessageHandler::OnCommandOutput(const face2wind::SerializeBase *data)
{
    Protocol::SCGOCommandOutput *output = (Protocol::SCGOCommandOutput *)data;
    UIManager::GetInstance().GetMainView()->AddOutputTxt(output);
}

void MessageHandler::OnCommandAck(const face2wind::SerializeBase *data)
{
    Protocol::SCGORequestCommandAck *cmd_ack = (Protocol::SCGORequestCommandAck *)data;

    if (OperateResultType_SUCC == cmd_ack->operate_result)
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("操作执行完毕！");
        UIManager::GetInstance().GetMainView()->SetUIEnable(true);
    }
    else if (OperateResultType_CANNOT_GET_LOCK == cmd_ack->operate_result)
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("您还有一个操作请求未执行完毕，请稍等！");
    }
}
