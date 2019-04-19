#include "LisASTMProtocol.h"
#include "LisData.h"

CLisASTMProtocol::CLisASTMProtocol()
    :CLisProtocol()
{
    m_eSessionState = eSessionState_Idle;
    m_sBarcode = "";
    m_sInstrumentInfo = "";
}

CLisASTMProtocol::~CLisASTMProtocol()
{

}


bool CLisASTMProtocol::UnpackData(QString sData)
{
    bool bRet = false;
    if (sData == ACK_ASTM)
        bRet = DealReplyACKData();
    else if (sData == ENQ_ASTM)
        bRet = DealReplyENQData();
    else if (sData == NAK_ASTM)
        bRet = DealReplyNAKData();
    else if (sData == EOT_ASTM)
        bRet = DealReplyEOTData();
    else
        bRet = DealReplyOtherData(sData);

    return true;
}


bool CLisASTMProtocol::StartSession()
{
    if (m_eSessionState != eSessionState_Idle)
        return false;

    QString sStart = ENQ_ASTM;
    emit SignalResponseData(sStart);
    m_eSessionState = eSessionState_InArbitrate;

    return true;
}


bool CLisASTMProtocol::EndSession()
{
    QString sEnd = EOT_ASTM;
    emit SignalResponseData(sEnd);

    m_eSessionState = eSessionState_Idle;
    return true;
}


bool CLisASTMProtocol::Unpack_RequstSa(QString sRequstInfo)
{
    vector<string> vecMainData;
    vecMainData.clear();
    SplitStrToArrayKeepWhiteSpaceA(sRequstInfo.toStdString(), vecMainData, "\r");
    if (vecMainData.size() < 3)
        return false;

    vector<string> vecHeaderData;
    vecHeaderData.clear();
    SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(0), vecHeaderData, "|");
    if (vecHeaderData.size() < 12)
        return false;

    if (vecHeaderData.at(11) != "RQ")
        return false;

    vector<string> vecRequstData;
    vecRequstData.clear();
    SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(1), vecRequstData, "|");
    if (vecRequstData.size() < 12)
        return false;

    m_sInstrumentInfo = QString::fromStdString(vecHeaderData.at(4));

    QString sTemp = QString::fromStdString(vecRequstData.at(3));
    int iPos = sTemp.indexOf("^");
    m_sBarcode = sTemp.mid(iPos + 1);

    return true;
}

bool CLisASTMProtocol::Unpack_QcResult(QString sQcResult)
{
    return true;
}

bool CLisASTMProtocol::Unpack_SaResult(QString sSaResult)
{
    // 分离
    vector<string> vecMainData;
    vecMainData.clear();
    SplitStrToArrayKeepWhiteSpaceA(sSaResult.toStdString(), vecMainData, "\r");
    if (vecMainData.size() < 4)
        return false;

    // 查看LIS是否存在对应条码的病人
    vector<string> vecOrderData;
    vecOrderData.clear();
    SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(2), vecOrderData, "|");
    if (vecOrderData.size() < 7)
        return false;

    QString sBarcode = QString::fromStdString(vecOrderData.at(3));
    bool bIsFind = CLisData::GetInstance()->IsFindByBarcode(sBarcode);
    return bIsFind;
}

bool CLisASTMProtocol::Response_SaInfo(bool bIsFind, const SLisPatientConv& LisPa)
{
    SLisSampleConv LisSa;
    SLisTestItemConv LisTestItem;
    if (LisPa.vecSample.size() >= 1)
        LisSa = LisPa.vecSample.at(0);

    QString sPaInfo = "";
    QString sCurTime = VAZGetNowDateTimeStrSuffix();
    sPaInfo += STX_ASTM;
    sPaInfo += "1H|\\^&|||" + m_sInstrumentInfo + "|||||||SA|1394-97|" + sCurTime + CR_ASTM;
    sPaInfo += "P|1||" + QString::fromStdWString(LisPa.tsHosNo) + "||" + QString::fromStdWString(LisPa.tsName) + "||";

    QString sBirthday = VAZDateTimeToStr(LisPa.fBirthday).remove(8, 6);
    sPaInfo += sBirthday + "|" + QString::fromStdWString(LisPa.tsSex) + "|||" + QString::fromStdWString(LisPa.tsBLDType) + "|||"
        + QString::fromStdWString(LisSa.tsSampAttr) + "||||||" + QString::fromStdWString(LisSa.tsBldNo) + "|||||" + QString::fromStdWString(LisPa.tsArea)
        + "|" + QString::fromStdWString(LisPa.tsBedNo) + "||||||||" + CR_ASTM;

    QString sSaPosAndId = QString::fromStdWString(LisSa.tsSampleNo) + "^" + QString::number(LisSa.iRack) + "^" + QString::number(LisSa.iCup);
    QString sItemInfo = "";
    vector<SLisTestItemConv>::const_iterator itTest = LisSa.vecTestItem.begin();
    for (; itTest != LisSa.vecTestItem.end(); itTest++)
    {
        if (itTest != LisSa.vecTestItem.begin())
            sItemInfo += "\\";

        sItemInfo += QString::fromStdWString(itTest->tsCode) + "^" + QString::fromStdWString(itTest->tsName) + "^^";
    }

    sPaInfo += "O|1|" + sSaPosAndId + "|" + QString::fromStdWString(LisSa.tsSampleID) + "|" + sItemInfo + "|" + (LisSa.bUrgent ? "S|" : "R|")
        + VAZDateTimeToStr(LisSa.fCheck) + "|" + VAZDateTimeToStr(LisSa.fCommit) + "|||||||" + VAZDateTimeToStr(LisSa.fCommit) + "|"
        + QString::fromStdWString(LisSa.tsSampleType) + "|" + QString::fromStdWString(LisSa.tsDoctor) + "|" + QString::fromStdWString(LisSa.tsCheckDept)
        + "||" + QString::fromStdWString(LisSa.tsChecker) + "||||||Q|||||" + CR_ASTM;

    sPaInfo += (bIsFind ? QObject::tr("L|1|N|") : QObject::tr("L|1|I|")) + CR_ASTM;

    int iSize = sPaInfo.size();
    sPaInfo += ETX_ASTM + QString::number(iSize) + CR_ASTM + LF_ASTM;

    emit SignalResponseData(sPaInfo);
    return true;
}

void CLisASTMProtocol::ReplyNormal()
{
    QString sNormal = ACK_ASTM;
    emit SignalResponseData(sNormal);
}

void CLisASTMProtocol::ReplyAbnormal()
{
    QString sAbnormal = NAK_ASTM;
    emit SignalResponseData(sAbnormal);
}


bool CLisASTMProtocol::DealReplyACKData()
{
    bool bRet = true;
    switch (m_eSessionState)
    {
    case eSessionState_Master:
        EndSession();
    break;
    case eSessionState_InArbitrate:
    case eSessionState_Idle:
    {
        m_eSessionState = eSessionState_Master;
        if (m_eSessionType == eLisSessionType_Request)
        {
            SLisPatientConv LisPa;
            bool bRet = CLisData::GetInstance()->GetPaInfoByBarcode(m_sBarcode, LisPa);
            Response_SaInfo(bRet, LisPa);

            m_sBarcode.clear();
        }
    }
    break;
    case eSessionState_Slave:       // 从模式只需回答ACK或NAK
    default: return false;//break;
    }

    return bRet;
}

bool CLisASTMProtocol::DealReplyENQData()
{
    switch (m_eSessionState)
    {
    case eSessionState_InArbitrate:
        ReplyAbnormal();
        break;
    case eSessionState_Idle:
        ReplyNormal();
        m_eSessionState = eSessionState_Slave;
        break;
    case eSessionState_Master:
    case eSessionState_Slave:
    default: return false;//break;
    }

    return true;
}

bool CLisASTMProtocol::DealReplyNAKData()
{
    switch (m_eSessionState)
    {
    case eSessionState_Master:
    case eSessionState_Slave:
    case eSessionState_InArbitrate:
    case eSessionState_Idle:
    default: return false;//break;
    }

    return true;
}

bool CLisASTMProtocol::DealReplyEOTData()
{
    switch (m_eSessionState)
    {
    case eSessionState_Slave:
        m_eSessionState = eSessionState_Idle;
        if (m_eSessionType == eLisSessionType_Request)
            StartSession();
        break;
    case eSessionState_Master:
    case eSessionState_InArbitrate:
    case eSessionState_Idle:
    default: return false;//break;
    }

    return true;
}

bool CLisASTMProtocol::DealReplyOtherData(QString sData)
{
    switch (m_eSessionState)
    {
    case eSessionState_Slave:
    {
        vector<string> vecMainData;
        vecMainData.clear();
        SplitStrToArrayKeepWhiteSpaceA(sData.toStdString(), vecMainData, "\r");
        if (vecMainData.size() < 3)
            return false;

        vector<string> vecHeaderData;
        vecHeaderData.clear();
        SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(0), vecHeaderData, "|");
        if (vecHeaderData.size() < 12)
            return false;

        bool bRet = true;
        QString sSessionType = QString::fromStdString(vecHeaderData.at(11));
        if (sSessionType.compare("PR") == 0)
        {
            m_eSessionType = eLisSessionType_SendSaRes;
            bRet = Unpack_SaResult(sData);
        }
        else if (sSessionType.compare("QR") == 0)
        {
            m_eSessionType = eLisSessionType_SendQcRes;
            bRet = Unpack_QcResult(sData);
        }
        else if (sSessionType.compare("RQ") == 0)
        {
            m_eSessionType = eLisSessionType_Request;
            bRet = Unpack_RequstSa(sData);
        }

        bRet ? ReplyNormal() : ReplyAbnormal();
    }
    break;
    case eSessionState_Master:
    case eSessionState_InArbitrate:
    case eSessionState_Idle:
    default: return false;//break;
    }

    return true;
}


QString CLisASTMProtocol::ConvQStrFromMsgType_ASTM(EMsgType_ASTM eMsgType)
{
    QString sMsg = "";
    switch (eMsgType)
    {
    case eMsgType_PR: sMsg = "PR"; break;
    case eMsgType_QR: sMsg = "QR"; break;
    case eMsgType_CR: sMsg = "CR"; break;
    case eMsgType_RQ: sMsg = "RQ"; break;
    case eMsgType_QA: sMsg = "QA"; break;
    case eMsgType_SA: sMsg = "SA"; break;
    default:break;
    }

    return sMsg;
}