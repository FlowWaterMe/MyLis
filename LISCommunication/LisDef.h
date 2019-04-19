#pragma once
#pragma execution_character_set("utf-8")

#include <tchar.h>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <comdef.h>
#include <QString>
#include <QSerialPort>

using namespace std;

#define  TEMP_BUFFER_SIZE 8192

const unsigned char SB = 0x0B;
const unsigned char EB = 0x1C;
const unsigned char CR = 0x0D;

#define  MSH_PERIOD 0
#define  MSGTYPE 8
#define  DSPNUM 28


const unsigned char ENQ_ASTM = 0x05;
const unsigned char ACK_ASTM = 0x06;
const unsigned char EOT_ASTM = 0x04;
const unsigned char NAK_ASTM = 0x15;
const unsigned char STX_ASTM = 0x02;
const unsigned char LF_ASTM = 0x0A;
const unsigned char CR_ASTM = 0x0D;
const unsigned char ETB_ASTM = 0x17;
const unsigned char ETX_ASTM = 0x03;

enum EMsgType_ASTM
{
    eMsgType_PR = 0,    // ���˲��Խ��
    eMsgType_QR,        // �ʿز��Խ��
    eMsgType_CR,        // �������������ã�
    eMsgType_RQ,        // ���������ѯ
    eMsgType_QA,        // ������ѯ��Ӧ
    eMsgType_SA         // ����������Ϣ
};

enum ELisSessionType
{
    eLisSessionType_Idle = 0,
    eLisSessionType_Request,        // ����������Ϣ
    eLisSessionType_SendQcRes,      // �����ʿؽ��
    eLisSessionType_SendSaRes       // �����������
};


enum ELisConType
{
    eLisConType_Serial = 0,
    eLisConType_TCP,
};

enum EProtocolType
{
    eProtocolType_HL7 = 0,
    eProtocolType_ASTM,
};

enum ETabPaInfo
{
    eTabPaInfo_No = 0,
    eTabPaInfo_Name,
    eTabPaInfo_Sex,
    eTabPaInfo_Birthday,
    eTabPaInfo_BLDType,
    eTabPaInfo_Country,
    eTabPaInfo_Nation,
    eTabPaInfo_Native,
    eTabPaInfo_TELNo,
    eTabPaInfo_Addr,
    eTabPaInfo_ZipCode,
    eTabPaInfo_PaType,
    eTabPaInfo_FeeType,
    eTabPaInfo_SSN,
    eTabPaInfo_CHNo,
    eTabPaInfo_Area,
    eTabPaInfo_HosNo,
    eTabPaInfo_BedNo,
    eTabPaInfo_CurDept,
    eTabPaInfo_Doc,
    eTabPaInfo_Diag,
    eTabPaInfo_Remark,
    eTabPaInfo_AnimalType,
    eTabPaInfo_End,
};

enum ETabSaInfo
{
    eTabSaInfo_No = 0,
    eTabSaInfo_SaNo,
    eTabSaInfo_Barcode,
    eTabSaInfo_Rack,
    eTabSaInfo_Cup,
    eTabSaInfo_SaType,
    eTabSaInfo_Urgent,
    eTabSaInfo_SaAttr,
    eTabSaInfo_Doc,
    eTabSaInfo_Room,
    eTabSaInfo_Commit,
    eTabSaInfo_Checker,
    eTabSaInfo_CheckDept,
    eTabSaInfo_CheckTime,
    eTabSaInfo_BldNo,
    eTabSaInfo_End,
};

enum ETabTestInfo
{
    eTabTestInfo_No = 0,
    eTabTestInfo_Code,
    eTabTestInfo_Name,
    eTabTestInfo_ResType,
    eTabTestInfo_QuantiRes,         // ����
    eTabTestInfo_QualiRes,          // ����
    eTabTestInfo_Unit,
    eTabTestInfo_RangeH,
    eTabTestInfo_RangeL,
    eTabTestInfo_QualiRef,
    eTabTestInfo_Flag,
    eTabTestInfo_BaseVal,
    eTabTestInfo_TestTime,
    eTabTestInfo_End,
};


struct SLisTestItemConv
{
    int			iNo;							//���
    wstring		tsCode;							//��ĿLISͨ���ţ�����Э��������590���ֽڣ�����û�б�Ҫ���ʺ��ʿؽ���е���Ŀͨ���ű���һ��
    wstring		tsName;							//��Ŀ����
    wstring		tsResultType;					//������� "ST" ������Ŀ, "NM"������Ŀ, "BOTH" �����붨������,  ע��һ������Ϊ��
    double		fResultVal;						//����������ֵ
    bool		bResultValNull;					//����������ֵ�Ƿ�Ϊ��
    wstring		tsResultVal;					//���Լ�����ֵ
    wstring		tsUnit;							//��λ
    double		fRangeH;			            //�����ο���Χ-����
    bool		bRangeHNull;					//�����ο���Χ-�����Ƿ�Ϊ��
    double		fRangeL;			            //�����ο���Χ-����
    bool		bRangeLNull;					//�����ο���Χ-�����Ƿ�Ϊ��
    wstring		tsRefValue;						//���Բο�ֵ
    wstring		tsFlag;							//�쳣��־ L-ƫ��	H-ƫ��		N-����
    double		fBaseVal;						//ԭʼ���
    bool		bBaseValNull;					//ԭʼ����Ƿ�Ϊ��
    double		fTestTime;						//����ʱ��

    SLisTestItemConv()
    {
        iNo = 0;
        tsCode = _T("");
        tsName = _T("");
        tsResultType = _T("");
        fResultVal = 0.0;
        bResultValNull = false;
        tsResultVal = _T("");
        tsUnit = _T("");
        fRangeH = 0.0;
        bRangeHNull = false;
        fRangeL = 0.0;
        bRangeLNull = false;
        tsRefValue = _T("");
        tsFlag = _T("");
        fBaseVal = 0.0;
        bBaseValNull = false;
        fTestTime = 0.0;
    }
};


struct SLisSampleConv
{
    int			iNo;							//���
    wstring		tsSampleNo;						//�������
    wstring		tsSampleID;						//��������
    int			iRack;							//�̺�
    int			iCup;				            //��λ��
    wstring		tsSampleType;					//��������	//��������Ĭ��ȡֵΪ��Ѫ��ȡֵ��serum����Ѫ��ȡֵ��plasma������Һȡֵ��urine�������ݲ�Ʒ�Ĳ�ͬ���û�Ҳ���Ը���Ϊ�Զ���ֵ
    bool		bUrgent;			            //����
    wstring		tsSampAttr;						//������״ ����ȡֵ��icterus������Ѫȡֵ��hemolysis����֬Ѫȡֵ��lipemia������������ƽ̨�ͻ����Զ���ֵ����ȡֵ
    wstring		tsDoctor;						//�ͼ���
    wstring		tsRoom;							//�ͼ����
    double		fCommit;			            //�ͼ�ʱ��
    wstring		tsChecker;						//������
    wstring		tsCheckDept;					//�������
    double		fCheck;							//����ʱ��
    wstring		tsBldNo;						//Ѫ�����

    int			iTestItemNum;
    vector<SLisTestItemConv> vecTestItem;

    SLisSampleConv()
    {
        iNo = 0;
        tsSampleNo = _T("");
        tsSampleID = _T("");
        iRack = 0;
        iCup = 0;
        tsSampleType = _T("");
        bUrgent = false;
        tsSampAttr = _T("");
        tsDoctor = _T("");
        tsRoom = _T("");
        fCommit = 0.0;
        tsChecker = _T("");
        tsCheckDept = _T("");
        fCheck = 0.0;
        tsBldNo = _T("");
        iTestItemNum = 0;
        vecTestItem.clear();
    }
};


struct SLisPatientConv
{
    int			iNo;				            //���
    wstring		tsPNo;							//���˱��----LISЭ��δʹ��
    wstring		tsName;							//����
    wstring		tsSex;							//�Ա� ��:M	ŮF	����O δ֪U
    double		fBirthday;						//��������
    wstring		tsBLDType;						//Ѫ��,Ѫ��(AѪ��ȡֵ��A����BѪ��ȡֵ��B����ABѪ��ȡֵ��AB����OѪ��ȡֵ��O������������ƽ̨�ͻ����Զ���ֵ����ȡֵ)
    wstring		tsCountry;						//����
    wstring		tsNation;						//����
    wstring		tsNative;						//����
    wstring		tsTELNo;						//�绰
    wstring		tsAddr;							//��ַ
    wstring		tsZipCode;						//�ʱ�
    wstring		tsPatientType;					//�������
    wstring		tsFeeType;						//�շ�����
    wstring		tsSSN;							//ҽ���ʺ�
    wstring		tsCHNo;							//������
    wstring		tsArea;							//����
    wstring		tsHosNo;						//סԺ��
    wstring		tsBedNo;						//����
    wstring		tsCurDept;						//���ƿ���
    wstring		tsDoctor;						//����ҽ��
    wstring		tsDiag;							//�ٴ����
    wstring		tsRemark;						//��ע��Ϣ
    wstring		tsCheck;						//�����Ա----LISЭ��δʹ��
    wstring		tsCheckResult;					//��˽��----LISЭ��δʹ��
    wstring		tsAnimalType;					//�������
    int			iSampleNum;
    vector<SLisSampleConv> vecSample;

    SLisPatientConv()
    {
        Clear();
    }
    
    void Clear()
    {
        iNo = 0;
        tsPNo = _T("");
        tsName = _T("");
        tsSex = _T("");
        fBirthday = 0;
        tsBLDType = _T("");
        tsCountry = _T("");
        tsNation = _T("");
        tsNative = _T("");
        tsTELNo = _T("");
        tsAddr = _T("");
        tsZipCode = _T("");
        tsPatientType = _T("");
        tsFeeType = _T("");
        tsSSN = _T("");
        tsCHNo = _T("");
        tsArea = _T("");
        tsHosNo = _T("");
        tsBedNo = _T("");
        tsCurDept = _T("");
        tsDoctor = _T("");
        tsDiag = _T("");
        tsRemark = _T("");
        tsCheck = _T("");
        tsCheckResult = _T("");
        tsAnimalType = _T("");
        iSampleNum = 0;
        vecSample.clear();
    }
};


QSerialPort::DataBits ConvToQtDataBit(int iCbxIndex);
QSerialPort::Parity ConvToQtParity(int iCbxIndex);
QSerialPort::StopBits ConvToQtStopBit(int iCbxIndex);
QSerialPort::FlowControl ConvToQtFlowControl(int iCbxIndex);

void SplitStrToArrayKeepWhiteSpaceA(const string& sToSplit, vector<string> &vecValue, const string& sDelimit);
double	VAZGetCurrentDateTime();

QString VAZGetNowDateTimeStrSuffix();   // ��ʽ20110101235959
QString	VAZDateTimeToStr(DATE dtTime);  //��ʽ20110101235959
