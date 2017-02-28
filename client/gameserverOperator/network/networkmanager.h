#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <set>

#include <QtNetwork>
#include <QObject>
#include <QTimer>

typedef quint16 Port;
typedef QHostAddress IPAddr;
typedef int NetworkID;

#pragma pack(push, 4)

struct NetPackerFixHeadMsgHeader
{
  int package_length;
};
#pragma pack(pop)
const static int NET_PACKER_FIXHEAD_HEADER_LENGTH = sizeof(NetPackerFixHeadMsgHeader);

class INetworkHandler
{
public:
    virtual void OnConnect(IPAddr ip, Port port, Port local_port, bool success) = 0;

    virtual void OnRecv(const char *data, int length) = 0;
    virtual void OnDisconnect() = 0;
};

class NetworkManager : public QTcpSocket
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = 0);
    virtual ~NetworkManager();

    static NetworkManager & GetInstance();

    void RegistHandler(INetworkHandler *handler);
    void UnregistHandler(INetworkHandler *handler);

    void SyncConnect(IPAddr ip, Port port);

    void Send(const char *data, int length);
    void Disconnect();
    void DelayReconnect();

protected slots:
    virtual void OnConnect();
    void DoHeadBodyRecv();
    void OnRecv();
    virtual void OnDisconnect();

    virtual void OnRecvPackage(char *data, int length);
    virtual void displayError(QAbstractSocket::SocketError);  //显示错误
    void OnReconnectTimerTimeOut();

private:
    std::set<INetworkHandler*> handler_set_;
    QTimer reconnect_timer;

    QByteArray receive_ba_;
    int body_length_;
    IPAddr last_connect_ip;
    Port last_connect_port;
};

#endif // NETWORKMANAGER_H
