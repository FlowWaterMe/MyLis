#include "LisData.h"


CLisData::CLisData()
{
    m_vecPatientTest.clear();
}

CLisData::~CLisData()
{

}

void CLisData::InitData()
{
    m_vecPatientTest.clear();
    QStringList sNameList;
    sNameList << QObject::tr("张三") << QObject::tr("李四") << QObject::tr("王五") << QObject::tr("小明") << QObject::tr("小飞");
    QStringList sAssayList;
    sAssayList << QObject::tr("SAA") << QObject::tr("CRP");
    for (int ii = 0; ii < 5; ii++)
    {
        SLisPatientConv PaInfo;
        PaInfo.iNo = ii + 1;
        PaInfo.tsPNo = _T("");
        PaInfo.tsName = sNameList.at(ii).toStdWString();
        PaInfo.tsSex = bool(ii % 2) ? _T("M") : _T("F");
        PaInfo.fBirthday = VAZGetCurrentDateTime() - ii * 2500 - 1;
        if(ii == 2) PaInfo.fBirthday = VAZGetCurrentDateTime() - 30;
        PaInfo.tsBLDType = bool(ii % 2) ? _T("A") : _T("AB");
        PaInfo.tsCountry = _T("中国");
        PaInfo.tsNation = _T("汉");
        PaInfo.tsNative = _T("广东");
        PaInfo.tsTELNo = _T("1234-567569") + QString::number(ii).toStdWString();
        PaInfo.tsAddr = _T("深圳");
        PaInfo.tsZipCode = _T("123456");
        PaInfo.tsPatientType = _T("");
        PaInfo.tsFeeType = _T("");
        PaInfo.tsSSN = _T("");
        PaInfo.tsCHNo = _T("");
        PaInfo.tsArea = _T("");
        PaInfo.tsHosNo = _T("");
        PaInfo.tsBedNo = _T("");
        PaInfo.tsCurDept = _T("");
        PaInfo.tsDoctor = _T("");
        PaInfo.tsDiag = _T("");
        PaInfo.tsRemark = _T("");
        PaInfo.tsCheck = _T("");
        PaInfo.tsCheckResult = _T("");
        PaInfo.tsAnimalType = _T("高级动物");
        PaInfo.iSampleNum = 1;

        SLisSampleConv SaInfo;
        SaInfo.iNo = 1;
        SaInfo.tsSampleNo = QString::number(ii + 1).toStdWString();
        SaInfo.tsSampleID = _T("Barcode") + QString::number(ii + 1).toStdWString();
        SaInfo.tsSampleType = bool(ii % 2) ? _T("serum") : _T("plasma");
        SaInfo.bUrgent = false;
        SaInfo.tsSampAttr = _T("");
        SaInfo.tsDoctor = _T("Doc1");
        SaInfo.tsRoom = _T("");
        SaInfo.fCommit = VAZGetCurrentDateTime();
        SaInfo.tsChecker = _T("");
        SaInfo.tsCheckDept = _T("");
        SaInfo.fCheck = 0.0;
        SaInfo.tsBldNo = _T("");
        SaInfo.iTestItemNum = ii % 2 + 1;

        for (int jj = 0; jj < SaInfo.iTestItemNum; jj++)
        {
            SLisTestItemConv TestInfo;
            TestInfo.iNo = jj + 1;
            TestInfo.tsCode = sAssayList.at(jj).toStdWString();
            TestInfo.tsName = sAssayList.at(jj).toStdWString();
            TestInfo.tsResultType = _T("NM");
            TestInfo.fResultVal = 0.0;
            TestInfo.bResultValNull = false;
            TestInfo.tsResultVal = _T("");
            TestInfo.tsUnit = _T("mg/L");
            TestInfo.fRangeH = 0.0;
            TestInfo.bRangeHNull = false;
            TestInfo.fRangeL = 0.0;
            TestInfo.bRangeLNull = false;
            TestInfo.tsRefValue = _T("");
            TestInfo.tsFlag = _T("");
            TestInfo.fBaseVal = 0.0;
            TestInfo.bBaseValNull = true;
            TestInfo.fTestTime = 0.0;
            SaInfo.vecTestItem.push_back(TestInfo);
        }

        PaInfo.vecSample.push_back(SaInfo);
        m_vecPatientTest.push_back(PaInfo);
    }
}

std::vector<SLisPatientConv> CLisData::GetPaInfoVec()
{
    return m_vecPatientTest;
}

bool CLisData::GetPaInfoByBarcode(const QString sBarcode, SLisPatientConv& PaInfo)
{
    if (sBarcode.isEmpty())
        return false;

    PaInfo.Clear();
    vector<SLisPatientConv>::iterator itPaVec = m_vecPatientTest.begin();
    for (; itPaVec != m_vecPatientTest.end(); itPaVec++)
    {
        vector<SLisSampleConv>::iterator itSaVec = itPaVec->vecSample.begin();
        for (; itSaVec != itPaVec->vecSample.end(); itSaVec++)
        {
            if (itSaVec->tsSampleID == sBarcode.toStdWString())
            {
                PaInfo = *itPaVec;
                return true;
            }
        }
    }

    return false;
}

bool CLisData::IsFindByBarcode(const QString sBarcode)
{
    if (sBarcode.isEmpty())
        return false;

    vector<SLisPatientConv>::iterator itPaVec = m_vecPatientTest.begin();
    for (; itPaVec != m_vecPatientTest.end(); itPaVec++)
    {
        vector<SLisSampleConv>::iterator itSaVec = itPaVec->vecSample.begin();
        for (; itSaVec != itPaVec->vecSample.end(); itSaVec++)
        {
            if (itSaVec->tsSampleID == sBarcode.toStdWString())
                return true;
        }
    }

    return false;
}

bool CLisData::UpdateResult(const QString sBarcode, const SLisTestItemConv& TestInfo)
{
    vector<SLisPatientConv>::iterator itPaVec = m_vecPatientTest.begin();
    for (; itPaVec != m_vecPatientTest.end(); itPaVec++)
    {
        vector<SLisSampleConv>::iterator itSaVec = itPaVec->vecSample.begin();
        for (; itSaVec != itPaVec->vecSample.end(); itSaVec++)
        {
            if (itSaVec->tsSampleID == sBarcode.toStdWString())
            {
                vector<SLisTestItemConv>::iterator itTestVec = itSaVec->vecTestItem.begin();
                for (; itTestVec != itSaVec->vecTestItem.end(); itTestVec++)
                {
                    if (itTestVec->tsCode == TestInfo.tsCode && itTestVec->tsName == TestInfo.tsName)
                    {
                        itTestVec->tsResultType = TestInfo.tsResultType;
                        itTestVec->fResultVal = TestInfo.fResultVal;
                        itTestVec->tsUnit = TestInfo.tsUnit;
                        itTestVec->fRangeH = TestInfo.fRangeH;
                        itTestVec->fRangeL = TestInfo.fRangeL;
                        itTestVec->tsFlag = TestInfo.tsFlag;
                        itTestVec->fBaseVal = TestInfo.fBaseVal;

                        itTestVec->tsResultVal = TestInfo.tsResultVal;
                        itTestVec->tsRefValue = TestInfo.tsRefValue;

                        itTestVec->bBaseValNull = TestInfo.bBaseValNull;
                        itTestVec->bRangeHNull = TestInfo.bRangeHNull;
                        itTestVec->bRangeLNull = TestInfo.bRangeLNull;
                        itTestVec->bResultValNull = TestInfo.bResultValNull;
                        itTestVec->fTestTime = TestInfo.fTestTime;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

