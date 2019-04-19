#pragma once

#include "LisProtocol.h"

class CLisHL7Protocol : public CLisProtocol
{
    Q_OBJECT
public:
    CLisHL7Protocol();
    ~CLisHL7Protocol();

private:
    enum EResponseType
    {
        eResponseType_ORU_R01 = 0,
        eResponseType_QRY_Q02,
        eResponseType_ACK_R03,
    };

public:
    bool UnpackData(QString);

private:
    bool Unpack_ORU_R01(QString);
    bool Unpack_QRY_Q02(QString);
    bool Unpack_ACK_R03(QString);

    bool Response_ACK_R01(bool bIsFind, QString sResType);
    bool Response_QCK_Q02(bool bIsFind);
    bool Response_DSR_Q03(QString sData);

    bool QCResult(const vector<string>& vecMainData);
    bool SampleResult(const vector<string>& vecMainData);

signals:
    void SignalOrgResponse(EResponseType, QString);

private slots:
    void SlotOrgResposnse(EResponseType, QString);

private:
    int m_iMsgID;
};