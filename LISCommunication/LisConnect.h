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
    // vector�洢˳�򣺶˿�/������/����λ/����λ/ֹͣλ/������/XON/XOFF ���� IP/�˿ں�
    virtual bool LisConnect(const vector<QString>& vecParam) { return true; };
    virtual bool LisDisconnect() { return true; };
    virtual bool IsConnect() { return true; };
    virtual bool WriteData(QString) { return true; };

signals:
    void SignalReceiveData(QString);
};