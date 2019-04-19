#include "LisHL7Protocol.h"
#include "LisDef.h"
#include "LisData.h"
#include <QDateTime>

CLisHL7Protocol::CLisHL7Protocol()
{
    m_iMsgID = 1;
    QObject::connect(this, &CLisHL7Protocol::SignalOrgResponse, this, &CLisHL7Protocol::SlotOrgResposnse);
}

CLisHL7Protocol::~CLisHL7Protocol()
{

}

bool CLisHL7Protocol::UnpackData(QString sData)
{
    if (sData.isEmpty())
        return true;

    // 检查头(SB)尾(EB)
    int iHead = sData.indexOf(SB);
    int iEnd = sData.indexOf(EB);
    if(-1 == iHead || -1 == iEnd)
        return false;

    // 去除头尾及回车
    sData.remove(iHead, 1);
    sData.remove(iEnd, 2);

    // 分离
    vector<string> vecMainData;
    vecMainData.clear();
    SplitStrToArrayKeepWhiteSpaceA(sData.toStdString(), vecMainData, "\r");
    if (vecMainData.size() < 0)
        return false;

    // 识别当前解析数据类型
    vector<string> vecSubData;
    vecSubData.clear();
    SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(MSH_PERIOD), vecSubData, "|");
    if (vecSubData.size() < 9)
        return false;

    bool bRes = false;
    std::string sMsgType = vecSubData.at(MSGTYPE);
    if ("ORU^R01" == sMsgType)
        bRes = Unpack_ORU_R01(sData);
    else if ("QRY^Q02" == sMsgType)
        bRes = Unpack_QRY_Q02(sData);
    else if ("ACK^R03" == sMsgType)
        bRes = Unpack_ACK_R03(sData);

    return bRes;
}

// ORU_R01分三段：MSH、PID、OBR、{OBX}
bool CLisHL7Protocol::Unpack_ORU_R01(QString sData)
{
    vector<string> vecMainData;
    vecMainData.clear();
    SplitStrToArrayKeepWhiteSpaceA(sData.toStdString(), vecMainData, "\r");
    if (vecMainData.size() < 2)
        return false;

    vector<string> vecMSHData;
    vecMSHData.clear();
    SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(MSH_PERIOD), vecMSHData, "|");
    if (vecMSHData.size() < 9)
        return false;
    m_iMsgID = QString::fromStdString(vecMSHData.at(9)).toInt();

    vector<string> vecPIDOrOBRData;
    vecPIDOrOBRData.clear();
    SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(MSH_PERIOD + 1), vecPIDOrOBRData, "|");
    if (vecPIDOrOBRData.size() < 1)
        return false;

    QString sPID = QString::fromStdString(vecPIDOrOBRData.at(0));
    bool bIsSampleRes = (sPID.compare("PID") == 0) ? true : false;
    if (bIsSampleRes)
        SampleResult(vecMainData);
    else
        QCResult(vecMainData);

    return true;
}

bool CLisHL7Protocol::Unpack_QRY_Q02(QString sData)
{
    vector<string> vecMainData;
    vecMainData.clear();
    SplitStrToArrayKeepWhiteSpaceA(sData.toStdString(), vecMainData, "\r");
    if (vecMainData.size() < 1)
        return false;

    vector<string> vecMSHData;
    vecMSHData.clear();
    SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(MSH_PERIOD), vecMSHData, "|");
    if (vecMSHData.size() < 9)
        return false;
    m_iMsgID = QString::fromStdString(vecMSHData.at(9)).toInt();

    vector<string> vecSubData;
    vecSubData.clear();
    SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(MSH_PERIOD + 1), vecSubData, "|");
    if (vecSubData.size() < 8)
        return false;
    QString sQryBarcode = QString::fromStdString(vecSubData.at(8));

    emit SignalOrgResponse(eResponseType_QRY_Q02, sQryBarcode);
    return true;
}

bool CLisHL7Protocol::Unpack_ACK_R03(QString sData)
{
    emit SignalOrgResponse(eResponseType_ACK_R03, sData);
    return true;
}


// sResType: 0 病人样本结果  2 质控结果
bool CLisHL7Protocol::Response_ACK_R01(bool bIsFind, QString sResType)
{
    QString sCurTime = VAZGetNowDateTimeStrSuffix();
    QString sResponseData = SB;
    sResponseData += "MSH|^~\\&|VAZYME|CRP|||" + sCurTime + "||ACK^R01|" + QString::number(m_iMsgID) + "|P|2.3.1||||" + sResType + "||ASCII|||\r";
    if(bIsFind)
        sResponseData += "MSA|AA|" + QString::number(m_iMsgID) + "|Message accepted|||0|\r" + EB + CR; 
    else
        sResponseData += "MSA|AR|" + QString::number(m_iMsgID) + "|Message refuse|||204|\r" + EB + CR;
    
    emit SignalResponseData(sResponseData);
    return true;
}

// QCK_Q02分四段：MSH、MSA、ERR、QAK
bool CLisHL7Protocol::Response_QCK_Q02(bool bIsFind)
{
    QString sCurTime = VAZGetNowDateTimeStrSuffix();
    QString sResponseData = SB;
    sResponseData += "MSH|^~\\&|||VAZYME|CRP|" + sCurTime + "||QCK^Q02|" + QString::number(m_iMsgID) + "|P|2.3.1||||||ASCII|||\r";
    sResponseData += "MSA|AA|" + QString::number(m_iMsgID) + "|Message accepted|||0|\r";
    sResponseData += QObject::tr("ERR|0|\r");
    if (bIsFind)
        sResponseData += QObject::tr("QAK|SR|OK|\r") + EB + CR;
    else
        sResponseData += QObject::tr("QAK|SR|NF|\r") + EB + CR;

    emit SignalResponseData(sResponseData);
    return true;
}

// DSR_Q03分八段：MSH、MSA、ERR、QAK、QRD、QRF、{DSP}、DSC
bool CLisHL7Protocol::Response_DSR_Q03(QString sBarcode)
{
    bool bIsFind = CLisData::GetInstance()->IsFindByBarcode(sBarcode);

    QString sCurTime = VAZGetNowDateTimeStrSuffix();
    QString sResponseData = SB;
    sResponseData += "MSH|^~\\&|||||" + sCurTime + "||DSR^Q03|" + QString::number(m_iMsgID) + "|P|2.3.1||||||ASCII|||\r";
    sResponseData += "MSA|AA|" + QString::number(m_iMsgID) + "|Message accepted|||0|\r";
    sResponseData += QObject::tr("ERR|0|\r");
    sResponseData += QObject::tr("QAK|SR|OK|\r");
    sResponseData += "QRD|" + sCurTime + "|R|D|1|||RD||OTH|||T|\r";
    sResponseData += "QRF|CRP|||||RCT|COR|ALL||\r";//" + sCurTime.left(8) + "000000  " + sCurTime + "

    SLisPatientConv PaInfo;
    CLisData::GetInstance()->GetPaInfoByBarcode(sBarcode, PaInfo);
    SLisSampleConv SaInfo = PaInfo.vecSample.at(0);

    int iID = 1;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + QString::fromStdWString(PaInfo.tsName) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + VAZDateTimeToStr(PaInfo.fBirthday) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + QString::fromStdWString(PaInfo.tsSex) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + QString::fromStdWString(PaInfo.tsBLDType) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + VAZDateTimeToStr(SaInfo.fCheck) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + QString::fromStdWString(PaInfo.tsCountry) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + QString::fromStdWString(SaInfo.tsSampleID) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + QString::fromStdWString(SaInfo.tsSampleNo) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + VAZDateTimeToStr(SaInfo.fCommit) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + (SaInfo.bUrgent ? "Y" : "N") + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "||" + QString::fromStdWString(SaInfo.tsSampleType) + "|||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;
    sResponseData += "DSP|" + QString::number(iID) + "|||||\r"; iID++;

    for (int ii = 0; ii < SaInfo.vecTestItem.size(); ii++, iID++)
    {
        sResponseData += "DSP|" + QString::number(iID) + "||"
            + QString::fromStdWString(SaInfo.vecTestItem.at(ii).tsCode) + "^"
            + QString::fromStdWString(SaInfo.vecTestItem.at(ii).tsName) + "^" 
            + QString::fromStdWString(SaInfo.vecTestItem.at(ii).tsUnit) + "^" 
            + QString::number(SaInfo.vecTestItem.at(ii).fRangeL) + "-" 
            + QString::number(SaInfo.vecTestItem.at(ii).fRangeH) + "|||\r";
    }

    sResponseData += QObject::tr("DSC||\r") + EB + CR;

    emit SignalResponseData(sResponseData);
    return true;
}

bool CLisHL7Protocol::QCResult(const vector<string>& vecMainData)
{
    Response_ACK_R01(true, "2");
    return true;
}

bool CLisHL7Protocol::SampleResult(const vector<string>& vecMainData)
{
    bool bRes = true;

    vector<string> vecOBRData;
    vecOBRData.clear();
    SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(MSH_PERIOD + 2), vecOBRData, "|");
    if (vecOBRData.size() < 2)
        return false;
    QString sBarcode = QString::fromStdString(vecOBRData.at(2));

    for (int ii = 0; ii < vecMainData.size() - 4; ii++)
    {
        vector<string> vecOBXData;
        vecOBXData.clear();
        SplitStrToArrayKeepWhiteSpaceA(vecMainData.at(MSH_PERIOD + 3 + ii), vecOBXData, "|");
        if (vecOBXData.size() < 0)
            return false;

        SLisTestItemConv TestInfo;
        TestInfo.tsResultType = QString::fromStdString(vecOBXData.at(2)).toStdWString();
        TestInfo.tsCode = QString::fromStdString(vecOBXData.at(3)).toStdWString();
        TestInfo.tsName = QString::fromStdString(vecOBXData.at(4)).toStdWString();

        double dDate = QDateTime::fromString("18991230000000", "yyyyMMddhhmmss").secsTo(QDateTime::fromString(QString::fromStdString(vecOBXData.at(14)), "yyyyMMddhhmmss"));
        TestInfo.fTestTime = dDate / 3600.0 / 24.0;
        if (TestInfo.tsResultType == _T("NM"))
        {
            TestInfo.fResultVal = QString::fromStdString(vecOBXData.at(5)).toDouble();
            TestInfo.tsUnit = QString::fromStdString(vecOBXData.at(6)).toStdWString();

            int iPos = vecOBXData.at(7).find("-");
            string sRef = vecOBXData.at(7);
            TestInfo.fRangeH = QString::fromStdString(sRef.substr(iPos + 1)).toDouble();
            TestInfo.fRangeL = QString::fromStdString(sRef.erase(iPos)).toDouble();

            TestInfo.tsFlag = QString::fromStdString(vecOBXData.at(8)).toStdWString();
            TestInfo.fBaseVal = QString::fromStdString(vecOBXData.at(13)).toDouble();
        }
        else
        {
            TestInfo.tsResultVal = QString::fromStdString(vecOBXData.at(9)).toStdWString();
            TestInfo.tsRefValue = QString::fromStdString(vecOBXData.at(10)).toStdWString();

            TestInfo.bBaseValNull = true;
            TestInfo.bRangeHNull = true;
            TestInfo.bRangeLNull = true;
            TestInfo.bResultValNull = true;
        }

        bRes = CLisData::GetInstance()->UpdateResult(sBarcode, TestInfo);
    }

    emit SignalOrgResponse(eResponseType_ORU_R01, sBarcode);
    return bRes;
}

void CLisHL7Protocol::SlotOrgResposnse(EResponseType eType, QString sBarcode)
{
    bool bIsFind = CLisData::GetInstance()->IsFindByBarcode(sBarcode);
    switch (eType)
    {
    case CLisHL7Protocol::eResponseType_ORU_R01:
        Response_ACK_R01(bIsFind, sBarcode.isEmpty() ? "2" : "0");                  // 
        break;
    case CLisHL7Protocol::eResponseType_QRY_Q02:
    {
        Response_QCK_Q02(bIsFind);
        if (!bIsFind) return;

        Response_DSR_Q03(sBarcode);
    }
        break;
    case CLisHL7Protocol::eResponseType_ACK_R03:
        break;
    default:
        break;
    }
}
