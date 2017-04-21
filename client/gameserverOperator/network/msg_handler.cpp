#include "msg_handler.h"
#include "protocol/cs_protocol_def.hpp"
#include "ui/ui_manager.hpp"
#include "networkagent.h"
#include <QtCore>
#include "ui/mainwindow.h"
#include "commondef.hpp"
#include "client_def.hpp"

using namespace Protocol;

MessageHandler::MessageHandler(QObject *parent) : QObject(parent), check_service_timer(this)
{
    handler_func_map_["SCCheckServiceInfoAck"] = &MessageHandler::OnCheckServiceInfoAck;
    handler_func_map_["SCGORequestCommandListACK"] = &MessageHandler::OnRequestCommandListACK;
    handler_func_map_["SCGOCommandOutput"] = &MessageHandler::OnCommandOutput;
    handler_func_map_["SCGORequestCommandAck"] = &MessageHandler::OnCommandAck;

    connect(&check_service_timer, SIGNAL(timeout()), this, SLOT(OnCheckServiceTimerTimeOut()));
}

MessageHandler::~MessageHandler()
{
    disconnect(&check_service_timer, SIGNAL(timeout()), this, SLOT(OnCheckServiceTimerTimeOut()));
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
    Protocol::SCCheckServiceInfoAck *ack = (Protocol::SCCheckServiceInfoAck*)data;
    if (0 == ack->port)
    {
        //qDebug()<<"MessageHandler::OnCheckServiceInfoAck receive port == 0";
        UIManager::GetInstance().GetMainView()->SetTipsTxt("游戏操作服务器未启动，等待它启动！");
        UIManager::GetInstance().GetMainView()->SetUIEnable(false);

        check_service_timer.start(2000);
        return;
    }

    NetworkAgent::GetInstance().Disconnect();
    NetworkAgent::GetInstance().ConnectToServer(IPAddr(ack->ip_addr.c_str()), ack->port);
}

void MessageHandler::OnRequestCommandListACK(const face2wind::SerializeBase *data)
{
    Protocol::SCGORequestCommandListACK *ack = (Protocol::SCGORequestCommandListACK*)data;
    UIManager::GetInstance().GetMainView()->SetOperateData(ack);

    check_service_timer.stop();
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
        UIManager::GetInstance().GetMainView()->OperateSucc();
    }
    else if (OperateResultType_CANNOT_GET_LOCK == cmd_ack->operate_result)
    {
        UIManager::GetInstance().GetMainView()->SetTipsTxt("您还有一个操作请求未执行完毕，请稍等！");
    }
    UIManager::GetInstance().GetMainView()->SetUIEnable(true);
}

void MessageHandler::OnCheckServiceTimerTimeOut()
{
    qDebug()<<"game server not online, request service list again.";

    Protocol::CSCheckServiceInfo check_info;
    check_info.service_type = ServiceType_GAME_OPERATOR;
    NetworkAgent::GetInstance().SendToServer(check_info);
}
