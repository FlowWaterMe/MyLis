#include "LISCommunication.h"
#include "LisConnect.h"
#include "LisProtocol.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <windows.h>
#include <tchar.h>
#include "LisData.h"

LISCommunication::LISCommunication(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    m_eConType = eLisConType_Serial;

    Init();
}

void LISCommunication::Init()
{
    ui.labStatus->setText(QObject::tr("状态：未连接!"));

    QStringList sList;
    sList << QObject::tr("") << QObject::tr("COM1") << QObject::tr("COM2") << QObject::tr("COM3")
        << QObject::tr("COM4") << QObject::tr("COM5") << QObject::tr("COM6") << QObject::tr("COM7")
        << QObject::tr("COM8") << QObject::tr("COM9") << QObject::tr("COM10") << QObject::tr("COM11")
        << QObject::tr("COM12") << QObject::tr("COM13") << QObject::tr("COM14") << QObject::tr("COM15")
        << QObject::tr("COM16") << QObject::tr("COM17") << QObject::tr("COM18") << QObject::tr("COM19")
        << QObject::tr("COM20");
    ui.cbxSerialNum->addItems(sList);
    ui.cbxSerialNum->setCurrentText(QObject::tr("COM2"));

    sList.clear();
    sList << QObject::tr("") << QObject::tr("110") << QObject::tr("300") << QObject::tr("600")
        << QObject::tr("1200") << QObject::tr("2400") << QObject::tr("4800") << QObject::tr("9600")
        << QObject::tr("14400") << QObject::tr("19200") << QObject::tr("38400") << QObject::tr("56000")
        << QObject::tr("57600") << QObject::tr("115200") << QObject::tr("128000") << QObject::tr("256000");
    ui.cbxBaudrate->addItems(sList);
    ui.cbxBaudrate->setCurrentText(QObject::tr("19200"));

    //数据位
    sList.clear();
    sList << QObject::tr("") << QObject::tr("5") << QObject::tr("6") << QObject::tr("7")
        << QObject::tr("8");
    ui.cbxDataBit->addItems(sList);
    ui.cbxDataBit->setCurrentText(QObject::tr("8"));

    //停止位
    sList.clear();
    sList << QObject::tr("") << QObject::tr("1") << QObject::tr("1.5") << QObject::tr("2");
    ui.cbxStopBit->addItems(sList);
    ui.cbxStopBit->setCurrentText(QObject::tr("1"));

    //校验位
    sList.clear();
    sList << QObject::tr("") << QObject::tr("无") << QObject::tr("奇校验") << QObject::tr("偶校验");
    ui.cbxParity->addItems(sList);
    ui.cbxParity->setCurrentText(QObject::tr("无"));

    //流控制
    sList.clear();
    sList << QObject::tr("") << QObject::tr("无") << QObject::tr("RCS/TCS") << QObject::tr("XON/XOFF");
    ui.cbxHandShake->addItems(sList);
    ui.cbxHandShake->setCurrentText(QObject::tr("无"));

    ui.edtXOFF->setText("17");
    ui.edtXON->setText("17");

    QRegExp rxIP("(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])");
    QValidator* va = new QRegExpValidator(rxIP, this);
    ui.edtIP->setValidator(va);
    ui.edtIP->setText("127.0.0.1");

    ui.edtPort->setValidator(new QIntValidator(1, 65535, this));
    ui.edtPort->setText("6000");

    InitTab();
    CLisData::GetInstance()->InitData();
    for(int ii = 0; ii < CLisData::GetInstance()->GetPaInfoVec().size(); ii++)
        AddDataToPaTab(CLisData::GetInstance()->GetPaInfoVec().at(ii));

    int iProtocol = 0;
    QString qsPathFile = "./cfg.ini";
    QFile File(qsPathFile);
    QTextStream outPath(&File);
    if (File.open(QIODevice::ReadOnly))
        iProtocol = outPath.readAll().toInt();
    File.close();

    QObject::connect(CLisConnect::GetSerialConInstance(), &CLisConnect::SignalReceiveData, this, &LISCommunication::SlotRecData);
    QObject::connect(CLisConnect::GetTCPConInstance(), &CLisConnect::SignalReceiveData, this, &LISCommunication::SlotRecData);
    QObject::connect(CLisProtocol::GetInstance(EProtocolType(iProtocol)), &CLisProtocol::SignalResponseData, this, &LISCommunication::SlotSendData);
}

void LISCommunication::on_tabPatientInfo_cellClicked(int iRow, int Col)
{
    if (iRow < 0 || iRow > ui.tabPatientInfo->rowCount())
        return;

    ui.tabSampleInfo->clearContents();
    ui.tabSampleInfo->setRowCount(0);

    ui.tabTestInfo->clearContents();
    ui.tabTestInfo->setRowCount(0);

    int iNo = ui.tabPatientInfo->item(iRow, eTabPaInfo_No)->text().toInt();
    // 更新界面
    vector<SLisPatientConv> vecPaInfo = CLisData::GetInstance()->GetPaInfoVec();
    vector<SLisPatientConv>::iterator itPaVec = vecPaInfo.begin();
    for (; itPaVec != vecPaInfo.end(); itPaVec++)
    {
        if (itPaVec->iNo == iNo)
        {
            vector<SLisSampleConv>::iterator itSaVec = itPaVec->vecSample.begin();
            for (; itSaVec != itPaVec->vecSample.end(); itSaVec++)
            {
                AddDataToSaTab(*itSaVec);

                vector<SLisTestItemConv>::iterator itTestVec = itSaVec->vecTestItem.begin();
                for (; itTestVec != itSaVec->vecTestItem.end(); itTestVec++)
                    AddDataToTestTab(*itTestVec);
            }

            break;
        }
    }
}

void LISCommunication::on_btnCon_clicked()
{
    int iConType = ui.tabWidget->currentIndex();
    if (CLisConnect::GetSerialConInstance()->IsConnect() || CLisConnect::GetTCPConInstance()->IsConnect())
    {
        ::MessageBox(NULL, _T("已有连接!\n重连或更换连接方式请先断开连接!"), _T("提示"), MB_ICONEXCLAMATION);
        return;
    }

    vector<QString> vecParam;
    vecParam.clear();

    // 连接参数检查
    if (eLisConType_Serial == iConType)
    {
        if (ui.cbxBaudrate->currentIndex() == 0 || ui.edtXOFF->text() == tr("")
            || ui.cbxDataBit->currentIndex() == 0 || ui.cbxHandShake->currentIndex() == 0
            || ui.cbxParity->currentIndex() == 0 || ui.cbxSerialNum->currentIndex() == 0
            || ui.cbxStopBit->currentIndex() == 0 || ui.edtXON->text() == tr(""))
        {
            ::MessageBox(NULL, _T("Lis 的串口参数有误!"), _T("警告"), MB_ICONEXCLAMATION);
            return;
        }

        vecParam.push_back(ui.cbxSerialNum->currentText());
        vecParam.push_back(ui.cbxBaudrate->currentText());
        vecParam.push_back(QString::number(ui.cbxDataBit->currentIndex()));
        vecParam.push_back(QString::number(ui.cbxParity->currentIndex()));
        vecParam.push_back(QString::number(ui.cbxStopBit->currentIndex()));
        vecParam.push_back(QString::number(ui.cbxHandShake->currentIndex()));
        vecParam.push_back(ui.edtXON->text());
        vecParam.push_back(ui.edtXOFF->text());
    }
    else
    {
        if (ui.edtIP->text() == tr("") || ui.edtPort->text() == tr(""))
        {
            ::MessageBox(NULL, _T("Lis TCP/IP 参数有误!"), _T("警告"), MB_ICONEXCLAMATION);
            return;
        }

        vecParam.push_back(ui.edtIP->text());
        vecParam.push_back(ui.edtPort->text());
    }

    if (!CLisConnect::GetInstance(ELisConType(iConType))->LisConnect(vecParam))
    {
        ::MessageBox(NULL, _T("连接失败!"), _T("错误"), MB_ICONHAND);
        return;
    }

    ::MessageBox(NULL, _T("连接成功!"), _T("提示"), MB_ICONEXCLAMATION);
    ui.labStatus->setText(QObject::tr("状态：已连接!"));
}

void LISCommunication::on_btnDisCon_clicked()
{
    bool bRes = true;
    if (CLisConnect::GetSerialConInstance()->IsConnect())
        bRes = CLisConnect::GetSerialConInstance()->LisDisconnect();

    if (CLisConnect::GetTCPConInstance()->IsConnect())
        bRes = CLisConnect::GetTCPConInstance()->LisDisconnect();

    ui.labStatus->setText(QObject::tr("状态：未连接!"));
    ::MessageBox(NULL, bRes ? _T("已断开连接!") : _T("断开连接失败!"), _T("提示"), MB_ICONEXCLAMATION);
}

void LISCommunication::on_btnClearText_clicked()
{
    ui.TEdtRecord->clear();
}

void LISCommunication::SlotSendData(QString sData)
{
    if (CLisConnect::GetSerialConInstance()->IsConnect())
    {
        bool bRes = CLisConnect::GetSerialConInstance()->WriteData(sData);
        ui.labPrompt->setText(bRes ? QObject::tr("提示: 串口发送数据成功!") : QObject::tr("提示: 串口发送数据失败!"));
    }

    if (CLisConnect::GetTCPConInstance()->IsConnect())
    {
        bool bRes = CLisConnect::GetTCPConInstance()->WriteData(sData);
        ui.labPrompt->setText(bRes ? QObject::tr("提示: TCP发送数据成功!") : QObject::tr("提示: TCP发送数据失败!"));
    }

    ui.TEdtRecord->append("[Send]: " + sData + "\n");
}

void LISCommunication::SlotRecData(QString sData)
{
    ui.TEdtRecord->append("[Receive]: " + sData + "\n");
    bool bRes = CLisProtocol::GetInstance(eProtocolType_HL7)->UnpackData(sData);
    ui.labPrompt->setText(bRes ? QObject::tr("提示: 解析数据成功!") : QObject::tr("提示: 解析数据失败!"));
}

void LISCommunication::InitTab()
{
    // 病人信息表
    ui.tabPatientInfo->verticalHeader()->setVisible(false);
    ui.tabPatientInfo->setColumnCount(eTabPaInfo_End);
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_No, new QTableWidgetItem(tr("编号")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Name, new QTableWidgetItem(tr("姓名")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Sex, new QTableWidgetItem(tr("性别")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Birthday, new QTableWidgetItem(tr("出生日期")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_BLDType, new QTableWidgetItem(tr("血型")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Country, new QTableWidgetItem(tr("国家"))); 
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Nation, new QTableWidgetItem(tr("民族")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Native, new QTableWidgetItem(tr("籍贯")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_TELNo, new QTableWidgetItem(tr("电话")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Addr, new QTableWidgetItem(tr("住址")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_ZipCode, new QTableWidgetItem(tr("邮编")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_PaType, new QTableWidgetItem(tr("病人类别")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_FeeType, new QTableWidgetItem(tr("收费类别")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_SSN, new QTableWidgetItem(tr("医保帐号")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_CHNo, new QTableWidgetItem(tr("病历号")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Area, new QTableWidgetItem(tr("病区")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_HosNo, new QTableWidgetItem(tr("住院号")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_BedNo, new QTableWidgetItem(tr("床号")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_CurDept, new QTableWidgetItem(tr("治疗科室")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Doc, new QTableWidgetItem(tr("主治医生")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Diag, new QTableWidgetItem(tr("临床诊断")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Remark, new QTableWidgetItem(tr("备注信息")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_AnimalType, new QTableWidgetItem(tr("动物类别")));

    ui.tabPatientInfo->setSelectionBehavior(QAbstractItemView::SelectRows);     //单击选择一行
    ui.tabPatientInfo->setSelectionMode(QAbstractItemView::SingleSelection);    //设置只能选择一行，不能多行选中
    ui.tabPatientInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);      //设置每行内容不可更改
    ui.tabPatientInfo->setAlternatingRowColors(true);                           //设置隔一行变一颜色，即：一灰一白

    for (int ii = eTabPaInfo_No; ii < eTabPaInfo_End; ii++)
        ui.tabPatientInfo->setColumnWidth(ii, 60);

    // 样本信息表
    ui.tabSampleInfo->verticalHeader()->setVisible(false);
    ui.tabSampleInfo->setColumnCount(eTabSaInfo_End);
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_No, new QTableWidgetItem(tr("编号")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_SaNo, new QTableWidgetItem(tr("样本编号")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Barcode, new QTableWidgetItem(tr("条码")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Rack, new QTableWidgetItem(tr("盘号")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Cup, new QTableWidgetItem(tr("杯位号")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_SaType, new QTableWidgetItem(tr("样本类型")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Urgent, new QTableWidgetItem(tr("急诊")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_SaAttr, new QTableWidgetItem(tr("样本性状")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Doc, new QTableWidgetItem(tr("送检者")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Room, new QTableWidgetItem(tr("送检科室")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Commit, new QTableWidgetItem(tr("送检时间")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Checker, new QTableWidgetItem(tr("检验者")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_CheckDept, new QTableWidgetItem(tr("检验科室")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_CheckTime, new QTableWidgetItem(tr("检验时间")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_BldNo, new QTableWidgetItem(tr("血袋编号")));

    ui.tabSampleInfo->setSelectionBehavior(QAbstractItemView::SelectRows);     //单击选择一行
    ui.tabSampleInfo->setSelectionMode(QAbstractItemView::SingleSelection);    //设置只能选择一行，不能多行选中
    ui.tabSampleInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);      //设置每行内容不可更改
    ui.tabSampleInfo->setAlternatingRowColors(true);                           //设置隔一行变一颜色，即：一灰一白

    for (int ii = eTabSaInfo_No; ii < eTabSaInfo_End; ii++)
        ui.tabSampleInfo->setColumnWidth(ii, 60);

    // 测试信息表
    ui.tabTestInfo->verticalHeader()->setVisible(false);
    ui.tabTestInfo->setColumnCount(eTabTestInfo_End);
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_No, new QTableWidgetItem(tr("编号")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_Code, new QTableWidgetItem(tr("项目LIS通道号")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_Name, new QTableWidgetItem(tr("项目名称")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_ResType, new QTableWidgetItem(tr("结果类型")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_QuantiRes, new QTableWidgetItem(tr("定量检验结果值")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_QualiRes, new QTableWidgetItem(tr("定性检验结果值")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_Unit, new QTableWidgetItem(tr("单位")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_RangeH, new QTableWidgetItem(tr("定量参考范围-上限")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_RangeL, new QTableWidgetItem(tr("定量参考范围-下限")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_QualiRef, new QTableWidgetItem(tr("定性参考值")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_Flag, new QTableWidgetItem(tr("异常标志")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_BaseVal, new QTableWidgetItem(tr("原始结果")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_TestTime, new QTableWidgetItem(tr("测试时间")));

    ui.tabTestInfo->setSelectionBehavior(QAbstractItemView::SelectRows);     //单击选择一行
    ui.tabTestInfo->setSelectionMode(QAbstractItemView::SingleSelection);    //设置只能选择一行，不能多行选中
    ui.tabTestInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);      //设置每行内容不可更改
    ui.tabTestInfo->setAlternatingRowColors(true);                           //设置隔一行变一颜色，即：一灰一白

    for(int ii = eTabTestInfo_No; ii < eTabSaInfo_End; ii++)
        ui.tabTestInfo->setColumnWidth(ii, 60);
}


void LISCommunication::AddDataToPaTab(const SLisPatientConv& PaInfo)
{
    QTableWidgetItem *widgetItem = NULL;
    int iRow = ui.tabPatientInfo->rowCount();
    ui.tabPatientInfo->setRowCount(iRow + 1);
    ui.tabPatientInfo->setRowHeight(iRow, 30);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::number(PaInfo.iNo));
    ui.tabPatientInfo->setItem(iRow, eTabPaInfo_No, widgetItem);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::fromStdWString(PaInfo.tsName));
    ui.tabPatientInfo->setItem(iRow, eTabPaInfo_Name, widgetItem);

    QStringList sList;
    sList << QObject::tr("") << QObject::tr("M") << QObject::tr("F") << QObject::tr("O");
    QComboBox* cbx = new QComboBox(this);
    cbx->addItems(sList);
    cbx->setCurrentText(QString::fromStdWString(PaInfo.tsSex));
    ui.tabPatientInfo->setCellWidget(iRow, eTabPaInfo_Sex, cbx);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(VAZDateTimeToStr(PaInfo.fBirthday));
    ui.tabPatientInfo->setItem(iRow, eTabPaInfo_Birthday, widgetItem);

    sList.clear();
    sList << QObject::tr("") << QObject::tr("A") << QObject::tr("B") << QObject::tr("AB") << QObject::tr("O");
    cbx = new QComboBox(this);
    cbx->addItems(sList);
    cbx->setCurrentText(QString::fromStdWString(PaInfo.tsBLDType));
    ui.tabPatientInfo->setCellWidget(iRow, eTabPaInfo_BLDType, cbx);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::fromStdWString(PaInfo.tsCountry));
    ui.tabPatientInfo->setItem(iRow, eTabPaInfo_Country, widgetItem);
}


void LISCommunication::AddDataToSaTab(const SLisSampleConv& SaInfo)
{
    QTableWidgetItem *widgetItem = NULL;
    int iRow = ui.tabSampleInfo->rowCount();
    ui.tabSampleInfo->setRowCount(iRow + 1);
    ui.tabSampleInfo->setRowHeight(iRow, 30);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::number(SaInfo.iNo));
    ui.tabSampleInfo->setItem(iRow, eTabSaInfo_No, widgetItem);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::fromStdWString(SaInfo.tsSampleNo));
    ui.tabSampleInfo->setItem(iRow, eTabSaInfo_SaNo, widgetItem);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::fromStdWString(SaInfo.tsSampleID));
    ui.tabSampleInfo->setItem(iRow, eTabSaInfo_Barcode, widgetItem);

    QStringList sList;
    sList << QObject::tr("") << QObject::tr("serum") << QObject::tr("plasma") << QObject::tr("urine");
    QComboBox* cbx = new QComboBox(this);
    cbx->addItems(sList);
    cbx->setCurrentText(QString::fromStdWString(SaInfo.tsSampleType));
    ui.tabSampleInfo->setCellWidget(iRow, eTabSaInfo_SaType, cbx);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(SaInfo.bUrgent ? "是" : "否");
    ui.tabSampleInfo->setItem(iRow, eTabSaInfo_Urgent, widgetItem);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(VAZDateTimeToStr(SaInfo.fCommit));
    ui.tabSampleInfo->setItem(iRow, eTabSaInfo_Commit, widgetItem);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(VAZDateTimeToStr(SaInfo.fCheck));
    ui.tabSampleInfo->setItem(iRow, eTabSaInfo_CheckTime, widgetItem);
}


void LISCommunication::AddDataToTestTab(const SLisTestItemConv& TestInfo)
{
    QTableWidgetItem *widgetItem = NULL;
    int iRow = ui.tabTestInfo->rowCount();
    ui.tabTestInfo->setRowCount(iRow + 1);
    ui.tabTestInfo->setRowHeight(iRow, 30);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::number(TestInfo.iNo));
    ui.tabTestInfo->setItem(iRow, eTabTestInfo_No, widgetItem);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::fromStdWString(TestInfo.tsCode));
    ui.tabTestInfo->setItem(iRow, eTabTestInfo_Code, widgetItem);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::fromStdWString(TestInfo.tsName));
    ui.tabTestInfo->setItem(iRow, eTabTestInfo_Name, widgetItem); 
    
    QStringList sList;
    sList << QObject::tr("") << QObject::tr("ST") << QObject::tr("NM") << QObject::tr("BOTH");
    QComboBox* cbx = new QComboBox(this);
    cbx->addItems(sList);
    cbx->setCurrentText(QString::fromStdWString(TestInfo.tsResultType));
    ui.tabTestInfo->setCellWidget(iRow, eTabTestInfo_ResType, cbx);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::number(TestInfo.fResultVal));
    ui.tabTestInfo->setItem(iRow, eTabTestInfo_QuantiRes, widgetItem);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::fromStdWString(TestInfo.tsUnit));
    ui.tabTestInfo->setItem(iRow, eTabTestInfo_Unit, widgetItem);


    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::number(TestInfo.fRangeH));
    ui.tabTestInfo->setItem(iRow, eTabTestInfo_RangeH, widgetItem);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::number(TestInfo.fRangeL));
    ui.tabTestInfo->setItem(iRow, eTabTestInfo_RangeL, widgetItem);

    sList.clear();
    sList << QObject::tr("") << QObject::tr("L") << QObject::tr("H") << QObject::tr("N");
    cbx = new QComboBox(this);
    cbx->addItems(sList);
    cbx->setCurrentText(QString::fromStdWString(TestInfo.tsFlag));
    ui.tabTestInfo->setCellWidget(iRow, eTabTestInfo_Flag, cbx);

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(QString::number(TestInfo.fBaseVal));
    ui.tabTestInfo->setItem(iRow, eTabTestInfo_BaseVal, widgetItem); 

    widgetItem = new QTableWidgetItem(QTableWidgetItem::Type);
    widgetItem->setText(VAZDateTimeToStr(TestInfo.fTestTime));
    ui.tabTestInfo->setItem(iRow, eTabTestInfo_TestTime, widgetItem);
}

