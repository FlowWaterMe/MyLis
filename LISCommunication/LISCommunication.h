#pragma once

#include <QtWidgets/QDialog>
#include "ui_LISCommunication.h"
#include "LisDef.h"

class LISCommunication : public QDialog
{
    Q_OBJECT

public:
    LISCommunication(QWidget *parent = Q_NULLPTR);
    void Init();

private slots:
    void on_tabPatientInfo_cellClicked(int iRow, int Col);
    void on_btnCon_clicked();
    void on_btnDisCon_clicked();
    void on_btnClearText_clicked();

    void SlotSendData(QString);
    void SlotRecData(QString);

private:
    void InitTab();
    void AddDataToPaTab(const SLisPatientConv&);
    void AddDataToSaTab(const SLisSampleConv&);
    void AddDataToTestTab(const SLisTestItemConv&);

private:
    Ui::LISCommunicationClass ui;
    ELisConType m_eConType;
};
