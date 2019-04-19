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
    ui.labStatus->setText(QObject::tr("״̬��δ����!"));

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

    //����λ
    sList.clear();
    sList << QObject::tr("") << QObject::tr("5") << QObject::tr("6") << QObject::tr("7")
        << QObject::tr("8");
    ui.cbxDataBit->addItems(sList);
    ui.cbxDataBit->setCurrentText(QObject::tr("8"));

    //ֹͣλ
    sList.clear();
    sList << QObject::tr("") << QObject::tr("1") << QObject::tr("1.5") << QObject::tr("2");
    ui.cbxStopBit->addItems(sList);
    ui.cbxStopBit->setCurrentText(QObject::tr("1"));

    //У��λ
    sList.clear();
    sList << QObject::tr("") << QObject::tr("��") << QObject::tr("��У��") << QObject::tr("żУ��");
    ui.cbxParity->addItems(sList);
    ui.cbxParity->setCurrentText(QObject::tr("��"));

    //������
    sList.clear();
    sList << QObject::tr("") << QObject::tr("��") << QObject::tr("RCS/TCS") << QObject::tr("XON/XOFF");
    ui.cbxHandShake->addItems(sList);
    ui.cbxHandShake->setCurrentText(QObject::tr("��"));

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
    // ���½���
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
        ::MessageBox(NULL, _T("��������!\n������������ӷ�ʽ���ȶϿ�����!"), _T("��ʾ"), MB_ICONEXCLAMATION);
        return;
    }

    vector<QString> vecParam;
    vecParam.clear();

    // ���Ӳ������
    if (eLisConType_Serial == iConType)
    {
        if (ui.cbxBaudrate->currentIndex() == 0 || ui.edtXOFF->text() == tr("")
            || ui.cbxDataBit->currentIndex() == 0 || ui.cbxHandShake->currentIndex() == 0
            || ui.cbxParity->currentIndex() == 0 || ui.cbxSerialNum->currentIndex() == 0
            || ui.cbxStopBit->currentIndex() == 0 || ui.edtXON->text() == tr(""))
        {
            ::MessageBox(NULL, _T("Lis �Ĵ��ڲ�������!"), _T("����"), MB_ICONEXCLAMATION);
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
            ::MessageBox(NULL, _T("Lis TCP/IP ��������!"), _T("����"), MB_ICONEXCLAMATION);
            return;
        }

        vecParam.push_back(ui.edtIP->text());
        vecParam.push_back(ui.edtPort->text());
    }

    if (!CLisConnect::GetInstance(ELisConType(iConType))->LisConnect(vecParam))
    {
        ::MessageBox(NULL, _T("����ʧ��!"), _T("����"), MB_ICONHAND);
        return;
    }

    ::MessageBox(NULL, _T("���ӳɹ�!"), _T("��ʾ"), MB_ICONEXCLAMATION);
    ui.labStatus->setText(QObject::tr("״̬��������!"));
}

void LISCommunication::on_btnDisCon_clicked()
{
    bool bRes = true;
    if (CLisConnect::GetSerialConInstance()->IsConnect())
        bRes = CLisConnect::GetSerialConInstance()->LisDisconnect();

    if (CLisConnect::GetTCPConInstance()->IsConnect())
        bRes = CLisConnect::GetTCPConInstance()->LisDisconnect();

    ui.labStatus->setText(QObject::tr("״̬��δ����!"));
    ::MessageBox(NULL, bRes ? _T("�ѶϿ�����!") : _T("�Ͽ�����ʧ��!"), _T("��ʾ"), MB_ICONEXCLAMATION);
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
        ui.labPrompt->setText(bRes ? QObject::tr("��ʾ: ���ڷ������ݳɹ�!") : QObject::tr("��ʾ: ���ڷ�������ʧ��!"));
    }

    if (CLisConnect::GetTCPConInstance()->IsConnect())
    {
        bool bRes = CLisConnect::GetTCPConInstance()->WriteData(sData);
        ui.labPrompt->setText(bRes ? QObject::tr("��ʾ: TCP�������ݳɹ�!") : QObject::tr("��ʾ: TCP��������ʧ��!"));
    }

    ui.TEdtRecord->append("[Send]: " + sData + "\n");
}

void LISCommunication::SlotRecData(QString sData)
{
    ui.TEdtRecord->append("[Receive]: " + sData + "\n");
    bool bRes = CLisProtocol::GetInstance(eProtocolType_HL7)->UnpackData(sData);
    ui.labPrompt->setText(bRes ? QObject::tr("��ʾ: �������ݳɹ�!") : QObject::tr("��ʾ: ��������ʧ��!"));
}

void LISCommunication::InitTab()
{
    // ������Ϣ��
    ui.tabPatientInfo->verticalHeader()->setVisible(false);
    ui.tabPatientInfo->setColumnCount(eTabPaInfo_End);
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_No, new QTableWidgetItem(tr("���")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Name, new QTableWidgetItem(tr("����")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Sex, new QTableWidgetItem(tr("�Ա�")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Birthday, new QTableWidgetItem(tr("��������")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_BLDType, new QTableWidgetItem(tr("Ѫ��")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Country, new QTableWidgetItem(tr("����"))); 
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Nation, new QTableWidgetItem(tr("����")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Native, new QTableWidgetItem(tr("����")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_TELNo, new QTableWidgetItem(tr("�绰")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Addr, new QTableWidgetItem(tr("סַ")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_ZipCode, new QTableWidgetItem(tr("�ʱ�")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_PaType, new QTableWidgetItem(tr("�������")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_FeeType, new QTableWidgetItem(tr("�շ����")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_SSN, new QTableWidgetItem(tr("ҽ���ʺ�")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_CHNo, new QTableWidgetItem(tr("������")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Area, new QTableWidgetItem(tr("����")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_HosNo, new QTableWidgetItem(tr("סԺ��")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_BedNo, new QTableWidgetItem(tr("����")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_CurDept, new QTableWidgetItem(tr("���ƿ���")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Doc, new QTableWidgetItem(tr("����ҽ��")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Diag, new QTableWidgetItem(tr("�ٴ����")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_Remark, new QTableWidgetItem(tr("��ע��Ϣ")));
    ui.tabPatientInfo->setHorizontalHeaderItem(eTabPaInfo_AnimalType, new QTableWidgetItem(tr("�������")));

    ui.tabPatientInfo->setSelectionBehavior(QAbstractItemView::SelectRows);     //����ѡ��һ��
    ui.tabPatientInfo->setSelectionMode(QAbstractItemView::SingleSelection);    //����ֻ��ѡ��һ�У����ܶ���ѡ��
    ui.tabPatientInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);      //����ÿ�����ݲ��ɸ���
    ui.tabPatientInfo->setAlternatingRowColors(true);                           //���ø�һ�б�һ��ɫ������һ��һ��

    for (int ii = eTabPaInfo_No; ii < eTabPaInfo_End; ii++)
        ui.tabPatientInfo->setColumnWidth(ii, 60);

    // ������Ϣ��
    ui.tabSampleInfo->verticalHeader()->setVisible(false);
    ui.tabSampleInfo->setColumnCount(eTabSaInfo_End);
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_No, new QTableWidgetItem(tr("���")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_SaNo, new QTableWidgetItem(tr("�������")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Barcode, new QTableWidgetItem(tr("����")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Rack, new QTableWidgetItem(tr("�̺�")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Cup, new QTableWidgetItem(tr("��λ��")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_SaType, new QTableWidgetItem(tr("��������")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Urgent, new QTableWidgetItem(tr("����")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_SaAttr, new QTableWidgetItem(tr("������״")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Doc, new QTableWidgetItem(tr("�ͼ���")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Room, new QTableWidgetItem(tr("�ͼ����")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Commit, new QTableWidgetItem(tr("�ͼ�ʱ��")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_Checker, new QTableWidgetItem(tr("������")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_CheckDept, new QTableWidgetItem(tr("�������")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_CheckTime, new QTableWidgetItem(tr("����ʱ��")));
    ui.tabSampleInfo->setHorizontalHeaderItem(eTabSaInfo_BldNo, new QTableWidgetItem(tr("Ѫ�����")));

    ui.tabSampleInfo->setSelectionBehavior(QAbstractItemView::SelectRows);     //����ѡ��һ��
    ui.tabSampleInfo->setSelectionMode(QAbstractItemView::SingleSelection);    //����ֻ��ѡ��һ�У����ܶ���ѡ��
    ui.tabSampleInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);      //����ÿ�����ݲ��ɸ���
    ui.tabSampleInfo->setAlternatingRowColors(true);                           //���ø�һ�б�һ��ɫ������һ��һ��

    for (int ii = eTabSaInfo_No; ii < eTabSaInfo_End; ii++)
        ui.tabSampleInfo->setColumnWidth(ii, 60);

    // ������Ϣ��
    ui.tabTestInfo->verticalHeader()->setVisible(false);
    ui.tabTestInfo->setColumnCount(eTabTestInfo_End);
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_No, new QTableWidgetItem(tr("���")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_Code, new QTableWidgetItem(tr("��ĿLISͨ����")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_Name, new QTableWidgetItem(tr("��Ŀ����")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_ResType, new QTableWidgetItem(tr("�������")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_QuantiRes, new QTableWidgetItem(tr("����������ֵ")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_QualiRes, new QTableWidgetItem(tr("���Լ�����ֵ")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_Unit, new QTableWidgetItem(tr("��λ")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_RangeH, new QTableWidgetItem(tr("�����ο���Χ-����")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_RangeL, new QTableWidgetItem(tr("�����ο���Χ-����")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_QualiRef, new QTableWidgetItem(tr("���Բο�ֵ")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_Flag, new QTableWidgetItem(tr("�쳣��־")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_BaseVal, new QTableWidgetItem(tr("ԭʼ���")));
    ui.tabTestInfo->setHorizontalHeaderItem(eTabTestInfo_TestTime, new QTableWidgetItem(tr("����ʱ��")));

    ui.tabTestInfo->setSelectionBehavior(QAbstractItemView::SelectRows);     //����ѡ��һ��
    ui.tabTestInfo->setSelectionMode(QAbstractItemView::SingleSelection);    //����ֻ��ѡ��һ�У����ܶ���ѡ��
    ui.tabTestInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);      //����ÿ�����ݲ��ɸ���
    ui.tabTestInfo->setAlternatingRowColors(true);                           //���ø�һ�б�һ��ɫ������һ��һ��

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
    widgetItem->setText(SaInfo.bUrgent ? "��" : "��");
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

