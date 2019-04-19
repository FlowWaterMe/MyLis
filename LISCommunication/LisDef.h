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
    eMsgType_PR = 0,    // 病人测试结果
    eMsgType_QR,        // 质控测试结果
    eMsgType_CR,        // 定标结果（不启用）
    eMsgType_RQ,        // 样本请求查询
    eMsgType_QA,        // 样本查询回应
    eMsgType_SA         // 样本申请信息
};

enum ELisSessionType
{
    eLisSessionType_Idle = 0,
    eLisSessionType_Request,        // 请求样本信息
    eLisSessionType_SendQcRes,      // 发送质控结果
    eLisSessionType_SendSaRes       // 发送样本结果
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
    eTabTestInfo_QuantiRes,         // 定量
    eTabTestInfo_QualiRes,          // 定性
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
    int			iNo;							//序号
    wstring		tsCode;							//项目LIS通道号，按照协议最多可以590个字节，但是没有必要，故和质控结果中的项目通道号保持一致
    wstring		tsName;							//项目名称
    wstring		tsResultType;					//结果类型 "ST" 定性项目, "NM"定量项目, "BOTH" 定性与定量都有,  注意一定不能为空
    double		fResultVal;						//定量检验结果值
    bool		bResultValNull;					//定量检验结果值是否为空
    wstring		tsResultVal;					//定性检验结果值
    wstring		tsUnit;							//单位
    double		fRangeH;			            //定量参考范围-上限
    bool		bRangeHNull;					//定量参考范围-上限是否为空
    double		fRangeL;			            //定量参考范围-下限
    bool		bRangeLNull;					//定量参考范围-下限是否为空
    wstring		tsRefValue;						//定性参考值
    wstring		tsFlag;							//异常标志 L-偏低	H-偏高		N-正常
    double		fBaseVal;						//原始结果
    bool		bBaseValNull;					//原始结果是否为空
    double		fTestTime;						//测试时间

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
    int			iNo;							//序号
    wstring		tsSampleNo;						//样本编号
    wstring		tsSampleID;						//样本条码
    int			iRack;							//盘号
    int			iCup;				            //杯位号
    wstring		tsSampleType;					//样本类型	//样本类型默认取值为：血清取值”serum”，血浆取值”plasma”，尿液取值”urine”，根据产品的不同，用户也可以更改为自定义值
    bool		bUrgent;			            //急诊
    wstring		tsSampAttr;						//样本性状 黄疸取值”icterus”，溶血取值”hemolysis”，脂血取值”lipemia”，其他根据平台客户的自定义值进行取值
    wstring		tsDoctor;						//送检者
    wstring		tsRoom;							//送检科室
    double		fCommit;			            //送检时间
    wstring		tsChecker;						//检验者
    wstring		tsCheckDept;					//检验科室
    double		fCheck;							//检验时间
    wstring		tsBldNo;						//血袋编号

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
    int			iNo;				            //序号
    wstring		tsPNo;							//病人编号----LIS协议未使用
    wstring		tsName;							//姓名
    wstring		tsSex;							//性别 男:M	女F	其它O 未知U
    double		fBirthday;						//出生日期
    wstring		tsBLDType;						//血型,血型(A血型取值’A’，B血型取值’B’，AB血型取值’AB’，O血型取值’O’，其他根据平台客户的自定义值进行取值)
    wstring		tsCountry;						//国家
    wstring		tsNation;						//民族
    wstring		tsNative;						//籍贯
    wstring		tsTELNo;						//电话
    wstring		tsAddr;							//地址
    wstring		tsZipCode;						//邮编
    wstring		tsPatientType;					//病人类别
    wstring		tsFeeType;						//收费类型
    wstring		tsSSN;							//医保帐号
    wstring		tsCHNo;							//病历号
    wstring		tsArea;							//病区
    wstring		tsHosNo;						//住院号
    wstring		tsBedNo;						//床号
    wstring		tsCurDept;						//治疗科室
    wstring		tsDoctor;						//主治医生
    wstring		tsDiag;							//临床诊断
    wstring		tsRemark;						//备注信息
    wstring		tsCheck;						//审核人员----LIS协议未使用
    wstring		tsCheckResult;					//审核结果----LIS协议未使用
    wstring		tsAnimalType;					//动物类别
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

QString VAZGetNowDateTimeStrSuffix();   // 格式20110101235959
QString	VAZDateTimeToStr(DATE dtTime);  //格式20110101235959
