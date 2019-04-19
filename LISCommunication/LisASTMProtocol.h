#pragma once

#include "LisProtocol.h"

class CLisASTMProtocol : public CLisProtocol
{
    Q_OBJECT
public:
    CLisASTMProtocol();
    ~CLisASTMProtocol();

public:
    bool UnpackData(QString);

private:
    enum ESessionState
    {
        eSessionState_Idle = 0,         // ø’œ–
        eSessionState_InArbitrate,      // ÷Ÿ≤√÷– 
        eSessionState_Master,           // ÷˜
        eSessionState_Slave             // ¥”
    };

public:
    bool StartSession();
    bool EndSession();

private:
    bool Unpack_RequstSa(QString sRequstInfo);
    bool Unpack_QcResult(QString sQcResult);
    bool Unpack_SaResult(QString sSaResult);

    bool Response_SaInfo(bool bIsFind, const SLisPatientConv& LisPa);

    void ReplyNormal();
    void ReplyAbnormal();

    bool DealReplyACKData();
    bool DealReplyENQData();
    bool DealReplyNAKData();
    bool DealReplyEOTData();
    bool DealReplyOtherData(QString sData);

    QString CLisASTMProtocol::ConvQStrFromMsgType_ASTM(EMsgType_ASTM eMsgType);

private:
    ESessionState m_eSessionState;
    ELisSessionType m_eSessionType;
    QString m_sBarcode;

    QString m_sInstrumentInfo;
};