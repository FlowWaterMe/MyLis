#include "LisDef.h"

QSerialPort::DataBits ConvToQtDataBit(int iCbxIndex)
{
    switch (iCbxIndex)
    {
    case 1: return QSerialPort::Data5;
    case 2: return QSerialPort::Data6;
    case 3: return QSerialPort::Data7;
    case 4: return QSerialPort::Data8;
    case 0:
    default: return QSerialPort::UnknownDataBits;
    }
}


QSerialPort::Parity ConvToQtParity(int iCbxIndex)
{
    switch (iCbxIndex)
    {
    case 1: return QSerialPort::NoParity;
    case 2: return QSerialPort::OddParity;
    case 3: return QSerialPort::EvenParity;
    case 0:
    default: return QSerialPort::UnknownParity;
    }
}

QSerialPort::StopBits ConvToQtStopBit(int iCbxIndex)
{
    switch (iCbxIndex)
    {
    case 1: return QSerialPort::OneStop;
    case 2: return QSerialPort::OneAndHalfStop;
    case 3: return QSerialPort::TwoStop;
    case 0:
    default: return QSerialPort::UnknownStopBits;
    }
}

QSerialPort::FlowControl ConvToQtFlowControl(int iCbxIndex)
{
    switch (iCbxIndex)
    {
    case 1: return QSerialPort::NoFlowControl;
    case 2: return QSerialPort::HardwareControl;
    case 3: return QSerialPort::SoftwareControl;
    case 0:
    default: return QSerialPort::UnknownFlowControl;
    }
}


void SplitStrToArrayKeepWhiteSpaceA(const string& sToSplit, vector<string> &vecValue, const string& sDelimit)
{
    int iBeginPos = 0;
    int iEndPos = sToSplit.find(sDelimit, iBeginPos);
    if (iEndPos == string::npos)
    {
        vecValue.push_back(sToSplit);
        return;
    }

    while (iEndPos != string::npos)
    {
        vecValue.push_back(sToSplit.substr(iBeginPos, iEndPos - iBeginPos));
        iBeginPos = iEndPos + sDelimit.size();
        iEndPos = sToSplit.find(sDelimit, iBeginPos);
    }

    if (iBeginPos < sToSplit.size())
    {
        vecValue.push_back(sToSplit.substr(iBeginPos, sToSplit.size()));
    }
}

double	VAZGetCurrentDateTime()
{
    SYSTEMTIME st;
    double dTime = 0;

    ::GetLocalTime(&st); //��ʱʹ�øú�����ȡϵͳʱ�䣬ע��ú�����ȡ��ʱ����15ms�����
    ::SystemTimeToVariantTime(&st, &dTime);

    return dTime;
}


// ��ʽ20110101235959
QString VAZGetNowDateTimeStrSuffix()
{
    SYSTEMTIME st;
    ::GetLocalTime(&st);

    TCHAR szTime[TEMP_BUFFER_SIZE] = { 0 };
    _sntprintf(szTime, TEMP_BUFFER_SIZE, _T("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour,
        st.wMinute, st.wSecond);

    return QString::fromStdWString(szTime);
}

QString	VAZDateTimeToStr(DATE dtTime)//��ʽ20110101235959
{
    SYSTEMTIME st;
    VariantTimeToSystemTime(dtTime, &st);

    TCHAR szTime[TEMP_BUFFER_SIZE] = { 0 };
    _sntprintf(szTime, TEMP_BUFFER_SIZE, _T("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour,
        st.wMinute, st.wSecond, st.wMilliseconds);

    return QString::fromStdWString(szTime);
}

