#pragma once

#include "LisConnect.h"
#include <QtSerialPort/QSerialPort>  
#include <QtSerialPort/QSerialPortInfo>  

class CLisSerialConnect: public CLisConnect
{
    Q_OBJECT
public:
    CLisSerialConnect();
    ~CLisSerialConnect();


public:
    bool LisConnect(const vector<QString>& vecParam);
    bool LisDisconnect();
    bool IsConnect();
    bool WriteData(QString);

private slots:
    void SlotReadData();

private:
    QSerialPort* m_Serial;
    bool m_bIsAllRead;
};