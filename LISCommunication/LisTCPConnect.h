#pragma once

#include "LisConnect.h"
#include <QTcpSocket>
#include <QTcpServer>

class CLisTCPConnect : public CLisConnect
{
    Q_OBJECT
public:
    CLisTCPConnect();
    ~CLisTCPConnect();


public:
    bool LisConnect(const vector<QString>& vecParam);
    bool LisDisconnect();
    bool IsConnect();
    bool WriteData(QString);

private slots:
    void SlotNewConnection();
    void SlotReadData();

private:
    QTcpSocket* m_TcpSocket;
    QTcpServer* m_TcpServer;
    bool m_bIsAllRead;
};
