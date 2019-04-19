#pragma once

#include "LisDef.h"

class CLisProtocol : public QObject
{
    Q_OBJECT
public:
    static CLisProtocol* GetInstance(EProtocolType eType);
    virtual ~CLisProtocol() {};
    CLisProtocol() {};

public:
    virtual bool UnpackData(QString) { return true; };

signals:
    void SignalResponseData(QString);
};