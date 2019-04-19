#pragma once

#include "LisDef.h"

class CLisData
{
public:
    static CLisData* GetInstance()
    {
        static CLisData Instance;
        return &Instance;
    }
    ~CLisData();

    void InitData();
    vector<SLisPatientConv> GetPaInfoVec();
    bool GetPaInfoByBarcode(const QString sBarcode, SLisPatientConv& PaInfo);

    bool IsFindByBarcode(const QString sBarcode);

    bool UpdateResult(const QString sBarcode, const SLisTestItemConv& TestInfo);

private:
    CLisData();
    vector<SLisPatientConv> m_vecPatientTest;
};