#pragma once

#include "LisDef.h"

class CLisConnect : public QObject
{
    Q_OBJECT
public:    
    static CLisConnect* GetInstance(ELisConType eType);
    static CLisConnect* GetSerialConInstance();
    static CLisConnect* GetTCPConInstance();
    CLisConnect();
    virtual ~CLisConnect();
   
public:
    // vector存储顺序：端口/波特率/数据位/检验位/停止位/流控制/XON/XOFF 或者 IP/端口号
    virtual bool LisConnect(const vector<QString>& vecParam) { return true; };
    virtual bool LisDisconnect() { return true; };
    virtual bool IsConnect() { return true; };
    virtual bool WriteData(QString) { return true; };

signals:
    void SignalReceiveData(QString);
};