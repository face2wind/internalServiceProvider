#ifndef MSG_HANDLER_HPP
#define MSG_HANDLER_HPP

#include <QObject>
#include <QTimer>
#include <map>
#include <elegance/memory/serialize/serialize_base.hpp>

class MessageHandler;
typedef void (MessageHandler::*MessageHandlerFunc)(const face2wind::SerializeBase *data);

class MessageHandler : public QObject
{
    Q_OBJECT

public:
    explicit MessageHandler(QObject *parent = 0);
    virtual ~MessageHandler();

    void OnRecv(const face2wind::SerializeBase *data);

protected:
    void OnCheckServiceInfoAck(const face2wind::SerializeBase *data);
    void OnRequestCommandListACK(const face2wind::SerializeBase *data);
    void OnCommandOutput(const face2wind::SerializeBase *data);
    void OnCommandAck(const face2wind::SerializeBase *data);

protected slots:
    void OnCheckServiceTimerTimeOut();

private:
    std::map<std::string, MessageHandlerFunc> handler_func_map_;
    QTimer check_service_timer;
};

#endif // NETWORKAGENT_H
